#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "LoadResources.h"
#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "Assimp/postprocess.h"
#include "Assimp/material.h"
#include "Renderer.h"
#include "Assimp/texture.h"
#include "Assimp/RemoveComments.h"
#include "Object.hpp"
#include "Model.h"
#include "Texture.h"
#include "TextureData.h"
#include "Object3DGraph.h"

static const char* gVertexShaderStr = R"GLSL(
// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

// Uniforms
uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uCam;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec2 vUV;
out vec3 Normal;

void main()
{
	vUV = aUV;
    gl_Position = uProj * uCam * uModel * vec4(aPosition, 1.0);
})GLSL";

static const char* gFragmentShaderStr = R"GLSL(
// Varyings
in vec2 vUV;


// Uniforms
uniform sampler2D uColorTexture;
uniform sampler2D uNormalMap;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(uColorTexture, vUV);
    //oColor = texture(uNormalMap, vUV);
	//oColor = vec4(vUV,1.0f,0.0f);
})GLSL";


namespace Resources::Loader
{

	ResourcesManager::ResourcesManager()
	{
		Shader defaultShader("./Resources/Shaders/DefaultShader.vert", "./Resources/Shaders/DefaultShader.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		m_shaders.emplace("Default", std::make_shared<Shader>(defaultShader));

		Shader normalMapShader("./Resources/Shaders/DefaultShader.vert", "./Resources/Shaders/DefaultShaderNormalMap.frag");
		m_shaders.emplace("NormalMapDefault", std::make_shared<Shader>(normalMapShader));
	}

	ResourcesManager::~ResourcesManager()
	{
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

	void ResourcesManager::Load3DObject(const char* fileName)
	{
		std::string Name = fileName;

		std::shared_ptr<Object3DGraph> scene;
		if (m_scenes.count(Name) == 0)
			if (!LoadAssimpScene(fileName))
				return;
		else
			scene = m_scenes[Name];
	}

	bool ResourcesManager::LoadAssimpScene(const char* fileName)
	{
		std::string Name = fileName;

		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate 
											| aiProcess_GenSmoothNormals 
											| aiProcess_JoinIdenticalVertices 
											| aiProcess_SplitLargeMeshes 
											| aiProcess_SortByPType 
											| aiProcess_ValidateDataStructure
											| aiProcess_CalcTangentSpace);
		if (!scene)
		{
			// std::cout "could not load file" << std::endl;
			return false;
		}


		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		if (Name.find(".obj") != std::string::npos)
		{
			LoadMeshsSceneInSingleMesh(scene, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
		}
		else if (Name.find(".fbx") != std::string::npos)
		{
			LoadMeshsScene(scene, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
			// std::cout "it's an Fbx" << std::endl;
		}

		aiReleaseImport(scene);

		return true;
	}

	void ResourcesManager::LoadMeshsScene(const aiScene* scene, const std::string& directory)
	{

		unsigned int indexLastMesh{ 0 };
		unsigned int lastNumIndices{ 0 };

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
			std::shared_ptr<Model> model = std::make_shared<Model>();

			std::string name = directory + mesh->mName.data;

			int numberOfSameKey{ LoadMeshsSceneCheckModelIsLoaded(modelData, model, name) };

			modelData->model = model;
			indexLastMesh = modelData->vertices.size();
			lastNumIndices = modelData->indices.size();

			modelData->LoadVertices(mesh);
			modelData->LoadIndices(mesh);

			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(scene, mesh, directory, numberOfSameKey);
			}
			modelData->stateVAO = EOpenGLLinkState::CANLINK;
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
			numberOfSameKey = 1;
			std::string baseName = name;
			name = baseName + std::to_string(numberOfSameKey);
			// change the value of name with the prevent value + the number 1
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

	void ResourcesManager::LoadMeshsSceneInSingleMesh(const aiScene* scene, const std::string& directory)
	{
		if (m_models.count(directory + scene->mMeshes[0]->mName.data) > 0)
			return;

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		unsigned int indexLastMesh{ 0 };

		modelData->model = model;
		m_models.emplace(directory + scene->mMeshes[0]->mName.data, model);
		m_modelsToLink.push_back(modelData);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			modelData->LoadVertices(mesh);

			modelData->LoadIndices(mesh, indexLastMesh);

			indexLastMesh += mesh->mNumVertices;
			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(scene, mesh, directory);
			}
		}
		modelData->stateVAO = EOpenGLLinkState::CANLINK;
	}

	void ResourcesManager::LoadaiMeshMaterial(const aiScene* scene, aiMesh* mesh, const std::string& directory, const int numberOfSameKey)
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

		materialOut->LoadMaterialFromaiMaterial(mat, directory, *this);

	}

	void ResourcesManager::LoadTexture(const std::string& keyName, std::shared_ptr<Texture>& texture)
	{
		if (m_textures.count(keyName) > 0)
		{
			texture = m_textures[keyName];
			return;
		}
		texture = std::make_shared<Texture>();

		texture->LoadTexture(keyName, *this);
	}

	void ResourcesManager::LoadSceneResources(const aiScene* scene, const std::string& fileName, const std::string& directory)
	{
		std::shared_ptr<Object3DGraph> sceneData = std::make_shared<Object3DGraph>();

		aiNode* rootNode = scene->mRootNode;
		if (fileName.find(".obj") != std::string::npos)
		{
			sceneData->SceneLoad(scene, rootNode, directory, true);
		}
		else
		{
			sceneData->SceneLoad(scene, rootNode, directory, false);
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
					it = m_texturesToLink.erase(it);
					break;
				case EOpenGLLinkState::CANTLINK:
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

	void ResourcesManager::LinkAllModelToOpenGl()
	{
		for (auto it = m_modelsToLink.begin();
			it != m_modelsToLink.end();)
		{

			switch ((*it)->stateVAO)
			{
				case EOpenGLLinkState::LOADPROBLEM:
					it = m_modelsToLink.erase(it);
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
}
