#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Assimp/cimport.h"
#include "Assimp/scene.h"
//#include "Assimp/Importer.hpp"
#include "Assimp/postprocess.h"
#include "Assimp/material.h"
#include "Assimp/texture.h"
#include "Assimp/RemoveComments.h"

#include "RootObject.hpp"
#include "LoadResources.h"
#include "Object.hpp"
#include "ScriptedComponent.h"
#include "Model.h"
#include "Material.h"
//#include "Texture.h"
//#include "TextureData.h"
#include "Object3DGraph.h"


namespace Resources::Loader
{

	ResourcesManager::ResourcesManager()
	//m_task {new Core::SystemManagement::TaskSystem()}
	{
		CreateShader("Default", "Resources\\Shaders\\DefaultShader.vert", "Resources\\Shaders\\DefaultShader.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("NormalMapDefault", "Resources\\Shaders\\DefaultShader.vert", "Resources\\Shaders\\DefaultShaderNormalMap.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("Wireframe", "Resources\\Shaders\\WireframeShader.vert", "Resources\\Shaders\\WireframeShader.frag");
		CreateShader("Skybox", "Resources\\Shaders\\SkyboxShader.vert", "Resources\\Shaders\\SkyboxShader.frag");

		Shader normalMapShader("Resources\\Shaders\\DefaultShader.vert", "Resources\\Shaders\\DefaultShaderNormalMap.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		m_shaders.emplace("NormalMapDefault", std::make_shared<Shader>(normalMapShader));

		Shader wireframeShader("Resources\\Shaders\\WireframeShader.vert", "Resources\\Shaders\\WireframeShader.frag");
		m_shaders.emplace("Wireframe", std::make_shared<Shader>(wireframeShader));

		Material defaultMat;
		defaultMat.ambientColor = {1.f, 1.f, 1.f};
		defaultMat.diffuseColor = { 1.f, 1.f, 1.f };
		defaultMat.specularColor = { 1.f, 1.f, 1.f };
		defaultMat.shininessStrength = { 0.f };
		defaultMat.shininess = { 1.f };
		defaultMat.shader = m_shaders["Wireframe"];
		defaultMat.shaderChoose = true;

		m_materials.emplace("Default", std::make_shared<Material>(defaultMat));

		LoadObjInModel("Cube",".\\Resources\\Models\\cube.obj");
		LoadObjInModel("Capsule",".\\Resources\\Models\\capsule.obj");
		LoadObjInModel("Quad",".\\Resources\\Models\\quad.obj");
		LoadObjInModel("Skybox",".\\Resources\\Models\\skybox.obj");
		LoadObjInModel("Sphere",".\\Resources\\Models\\sphere.obj");
	}

	ResourcesManager::~ResourcesManager()
	{
		m_task.EndTaskSystem();

		for (unorderedmapModel::iterator itmodel = m_models.begin();
			itmodel != m_models.end(); ++itmodel)
			glDeleteBuffers(1, &itmodel->second->VAOModel);

		for (unorderedmapTexture::iterator ittexture = m_textures.begin();
			ittexture != m_textures.end(); ++ittexture)
			glDeleteTextures(1, &ittexture->second->texture);

		for (unorderedmapShader::iterator itshader = m_shaders.begin();
			itshader != m_shaders.end(); ++itshader)
			itshader->second->Delete();
	}

	void ResourcesManager::Load3DObject(const char* fileName, const bool graphInMulti)
	{
		std::string Name = fileName;

		if (m_scenes.count(Name) == 0)
			if (!LoadAssimpScene(fileName, graphInMulti))
				return;
	}

	const aiScene* ResourcesManager::LoadSceneFromImporter(Assimp::Importer& importer,const char* fileName)
	{
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		//aiImportFile
		const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate // load the 3DObject with only triangle
			//| aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices // join all vertices wich are the same for use indices for draw
			| aiProcess_SplitLargeMeshes
			| aiProcess_SortByPType
			| aiProcess_ValidateDataStructure // validate the scene data
			| aiProcess_CalcTangentSpace // calculate the tangent
			| aiProcess_GenBoundingBoxes // generate the AABB of the meshs aiProcess_Gen
			//| aiProcess_FlipUVs
		);

		return scene;
	}

	bool ResourcesManager::LoadAssimpScene(const char* fileName, const bool graphInMulti)
	{
		std::string Name = fileName;

		std::shared_ptr<ImporterData> importer = std::make_shared<ImporterData>();
		const aiScene* scene = LoadSceneFromImporter(importer->importer, fileName);

		if (!scene)
		{
			return false;
		}
		m_importerToDelete.push_back(importer);

		auto index = Name.find_last_of('/');
		if (index == std::string::npos)
			index = Name.find_last_of('\\');
		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		if (Name.find(".obj") != std::string::npos)
		{
			LoadMeshsSceneInSingleMesh(importer, scene, directoryFile);
			LoadSceneResources(importer, scene, Name, directoryFile, graphInMulti);
		}
		else if (Name.find(".fbx") != std::string::npos)
		{
			LoadMeshsScene(importer, scene, directoryFile);
			LoadSceneResources(importer, scene, Name, directoryFile, graphInMulti);
		}

		return true;
	}

	void ResourcesManager::LoadMeshsScene(std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, const std::string& directory)
	{

		unsigned int indexLastMesh{ 0 };
		unsigned int lastNumIndices{ 0 };

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
			std::shared_ptr<Model> model = std::make_shared<Model>();

			std::string name = directory + mesh->mName.data;

			int numberOfSameKey{ LoadMeshsSceneCheckModelIsLoaded(modelData, model, name) };

			modelData->model = model;
			modelData->SetArrays(scene, i);
			indexLastMesh = static_cast<unsigned int>(modelData->vertices.size());
			lastNumIndices = static_cast<unsigned int>(modelData->indices.size());

			m_task.AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, 0, 0);
			//modelData->LoadaiMeshModel(mesh);

			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(importer, scene, mesh, directory, numberOfSameKey);
			}
		}
	}

	int ResourcesManager::LoadMeshsSceneCheckModelIsLoaded(std::shared_ptr<ModelData>& currModelData, std::shared_ptr<Model>& currModel, const std::string& nameMesh)
	{
		int numberOfSameKey{ 0 };
		std::string name = nameMesh;
		// check if the name of the mesh isn't already a key
		if (m_models.count(nameMesh) > 0)
		{
			// the name of the mesh is already a key 
			// the mesh have the same name of an other mesh but it's not the same mesh
			// so we add a number to the name of the mesh for create a key
			numberOfSameKey++;
			std::string baseName = name;
			name = baseName + std::to_string(numberOfSameKey);
			// change the value of name with the prevent value + numberOfSameKey
			name = baseName + std::to_string(numberOfSameKey);

			// while the the key given to the unordered_map find an element
			// increase the number of same key
			while (m_models.count(name) > 0)
			{
				numberOfSameKey++;
				// chage the name with the new number of same key
				name = baseName + std::to_string(numberOfSameKey);
			}


			currModelData->ModelName = name;
			m_modelsToLink.push_back(currModelData);
			m_models.emplace(name, currModel);
		}
		else
		{
			currModelData->ModelName = name;
			m_modelsToLink.push_back(currModelData);
			m_models.emplace(name, currModel);
		}
		return numberOfSameKey;
	}

	void ResourcesManager::LoadMeshsSceneInSingleMesh(std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, const std::string& directory)
	{
		if (m_models.count(directory + scene->mMeshes[0]->mName.data) > 0)
			return;

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		unsigned int indexLastMesh{ 0 };

		modelData->model = model;
		modelData->ModelName = directory + scene->mMeshes[0]->mName.data;
		m_models.emplace(directory + scene->mMeshes[0]->mName.data, model);
		m_modelsToLink.push_back(modelData);

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			modelData->SetArrays(scene, i);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			m_task.AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, i, indexLastMesh);
			//modelData->LoadaiMeshModel(mesh, i,indexLastMesh);

			indexLastMesh += mesh->mNumVertices;
			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(importer, scene, mesh, directory);
			}
		}
	}

	void ResourcesManager::LoadObjInModel(const std::string& name, const char* fileName)
	{
		if (m_models.count(name) > 0)
			return;

		std::string Name = fileName;

		std::shared_ptr<ImporterData> importer = std::make_shared<ImporterData>();
		const aiScene* scene = LoadSceneFromImporter(importer->importer, fileName);

		if (!scene)
		{
			return;
		}
		
		m_importerToDelete.push_back(importer);

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		modelData->model = model;
		m_models.emplace(name, model);
		m_modelsToLink.push_back(modelData);
			
		aiMesh* mesh = scene->mMeshes[0];

		modelData->SetArrays(scene, 0);

		m_task.AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, 0, 0);

		std::shared_ptr<Object3DGraph> sceneData = std::make_shared<Object3DGraph>();

		sceneData->rootNodeScene.nameMesh = name;
		sceneData->rootNodeScene.nameObject = name;
		sceneData->rootNodeScene.namesMaterial.push_back("Default");
		m_scenes.emplace(fileName, sceneData);
		//modelData->LoadaiMeshModel(mesh, importer);
	}

	void ResourcesManager::LoadaiMeshMaterial(std::shared_ptr<Loader::ImporterData> importer, 
												const aiScene* scene, aiMesh* mesh, 
												const std::string& directory, const int numberOfSameKey)
	{	
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		Material material;
		std::string keyMaterial;

		if (numberOfSameKey > 0)
			keyMaterial = directory + mat->GetName().data + std::to_string(numberOfSameKey);
		else
			keyMaterial = directory + mat->GetName().data;

		if (m_materials.count(keyMaterial) > 0)
		{
			return;
		}

		std::shared_ptr<Material> materialOut = std::make_shared<Material>();

		m_materials.emplace(keyMaterial, materialOut);

		m_materialsGetUniform.push_back(materialOut);

		m_task.AddTask(&Resources::Material::LoadMaterialFromaiMaterial, materialOut.get(), mat, directory, this, importer);

	}

	void ResourcesManager::LoadTexture(const std::string& keyName, std::shared_ptr<Texture>& texture)
	{
		if (m_textures.count(keyName) > 0)
		{
			texture = m_textures[keyName];
			return;
		}

		std::shared_ptr<TextureData> textureData = std::make_shared<TextureData>();

		/*if (!texture)
			texture = std::make_shared<Texture>();*/

		if (!texture)
		{
			// just create a simple texture
			texture = std::make_shared<Texture>();
			textureData->oldTextureptr = texture;
		}
		else
		{
			// link the current texture we want to change
			textureData->oldTextureptr = texture;
			// texture is make_skared for create a new one
			texture = std::make_shared<Texture>();
		}
		m_textures.emplace(keyName, texture);

		texture->name = keyName;
		textureData->nameTexture = keyName;
		PushTextureToLink(textureData);
		textureData->textureptr = texture;
		//m_task.AddTask(&Resources::TextureData::CreateTextureFromImage, textureData, keyName, this, true);	
		//textureData->CreateTextureFromImage(keyName, this);

	}

	void ResourcesManager::LoadSceneResources(std::shared_ptr<Loader::ImporterData>& importer, 
												const aiScene* scene, const std::string& fileName, 
												const std::string& directory, const bool inMultiThread)
	{
		std::shared_ptr<Object3DGraph> sceneData = std::make_shared<Object3DGraph>();

		aiNode* rootNode = scene->mRootNode;
		if (fileName.find(".obj") != std::string::npos)
		{
			if(inMultiThread)
				m_task.AddTask(&Resources::Object3DGraph::SceneLoad, sceneData.get(), importer,scene, rootNode, directory, true);
			else
				sceneData->SceneLoad(importer, scene, rootNode, directory, true);
			
		}
		else
		{
			if (inMultiThread)
				m_task.AddTask(&Resources::Object3DGraph::SceneLoad, sceneData.get(), importer, scene, rootNode, directory, false);
			else
				sceneData->SceneLoad(importer, scene, rootNode, directory, false);
		}

		m_scenes.emplace(fileName, sceneData);
	}

	void ResourcesManager::LinkAllTextureToOpenGl()
	{

		for (auto it = m_texturesToLink.begin(); 
				it != m_texturesToLink.end();)
		{
			switch ((*it)->stateTexture)
			{
				case EOpenGLLinkState::LOADPROBLEM:
					m_textures.erase((*it)->nameTexture);
					//(*it)->textureptr.reset((*it)->oldTextureptr.get());
					it = m_texturesToLink.erase(it);
					break;
				case EOpenGLLinkState::CANTLINK:
					if ((*it)->ID == 0)
					{
						glGenTextures(1, &(*it)->textureptr->texture);
						(*it)->ID = (*it)->textureptr->texture;
						m_task.AddTask(&Resources::TextureData::CreateTextureFromImage, (*it), (*it)->nameTexture.c_str(), this, true);
					}
					it++;
					break;
				case EOpenGLLinkState::ISLINK:
					it = m_texturesToLink.erase(it);
					break;
				case EOpenGLLinkState::CANLINK:
					(*it)->CreateOpenGLTexture();
					it = m_texturesToLink.erase(it);
					break;
			}
		}
	}

	void ResourcesManager::MaterialGetUniformFromShader()
	{
		for (auto it = m_materialsGetUniform.begin();
			it != m_materialsGetUniform.end();)
		{
			if ((*it)->shaderChoose)
			{
				(*it)->InitVariantUniform();
				it = m_materialsGetUniform.erase(it);
			}
		}
	}

	void ResourcesManager::LinkAllModelToOpenGl()
	{
		for (auto it = m_modelsToLink.begin();
			it != m_modelsToLink.end();)
		{

			switch ((*it)->stateVAO)
			{
				case EOpenGLLinkState::LOADPROBLEM:
					//it = m_modelsToLink.erase(it);
					it++;
					break;
				case EOpenGLLinkState::CANTLINK:
					it++;
					break;
				case EOpenGLLinkState::ISLINK:
					it = m_modelsToLink.erase(it);
					break;
				case EOpenGLLinkState::CANLINK:
					(*it)->CreateVAOModel();
					it = m_modelsToLink.erase(it);
					break;
				
			}
		}
	}



	void ResourcesManager::CheckDeleteAssimpImporter()
	{
		for (auto it = m_importerToDelete.begin();
			it != m_importerToDelete.end();)
		{
			if ((*it)->maxUseOfImporter == 0)
			{
				std::cout << (*it)->maxUseOfImporter << std::endl;
				it = m_importerToDelete.erase(it);
			}
			else
				it++;
		}
	}

	void ResourcesManager::UpdateResourcesManager()
	{
		LinkAllTextureToOpenGl();
		LinkAllModelToOpenGl();
		MaterialGetUniformFromShader();
		CheckDeleteAssimpImporter();
		ShaderUpdate();
	}

	std::shared_ptr<Shader> ResourcesManager::CreateShader(const char* shaderName, const char* vertexFilePath, const char* fragmentFilePath, Shader::EShaderHeaderType header)
	{
		if (m_shaders.find(shaderName) != m_shaders.end())
			return m_shaders[shaderName];

		Shader newShader(vertexFilePath, fragmentFilePath, header);
		std::shared_ptr<Shader> newPtr = std::make_shared<Shader>(newShader);
		m_shaders.emplace(shaderName, newPtr);

		return newPtr;
	}

	void ResourcesManager::ShaderUpdate()
	{
		for (unorderedmapShader::iterator itshader = m_shaders.begin();
			itshader != m_shaders.end(); ++itshader)
		{
			itshader->second->UseProgram();
			itshader->second->SendLights();
		}
	}

	void ResourcesManager::ReloadShaders()
	{
		for (unorderedmapShader::iterator itshader = m_shaders.begin();
			itshader != m_shaders.end(); ++itshader)
		{
			itshader->second->Reload();
		}
	}

	void ResourcesManager::ReloadScripts()
	{
		if (!m_rootNode)
			BAKERS_LOG_ERROR("Can not reload scripts: no root node found from Resources Manager");

		std::list<Core::Datastructure::ScriptedComponent*> scripts; 
		scripts = m_rootNode->GetComponentsOfType<Core::Datastructure::ScriptedComponent>();

		for (const auto& script : scripts)
			script->LoadLuaScript();
	}
}
