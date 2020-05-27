#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <imgui\imgui.h>

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
//#include "Material.h"
//#include "Texture.h"
//#include "TextureData.h"

#include "AudioClip.h"
#include "Object3DGraph.h"
#include "BoneData.h"
#include "Mat.hpp"


namespace Resources::Loader
{

	ResourcesManager::ResourcesManager():
	m_task {new Core::SystemManagement::TaskSystem()}
	{
		DefaultConstruct();
	}

	ResourcesManager::ResourcesManager(Core::SystemManagement::TaskSystem* task) :
		m_task{ task }
	{
		DefaultConstruct();
	}

	void ResourcesManager::DefaultConstruct()
	{
		CreateShader("DefaultNoTexture", ".\\Resources\\Shaders\\DefaultShader.vert", ".\\Resources\\Shaders\\DefaultNoTexture.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("Default", ".\\Resources\\Shaders\\DefaultShader.vert", ".\\Resources\\Shaders\\DefaultShader.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("SkeletDefault", ".\\Resources\\Shaders\\SkeletalShader.vert", ".\\Resources\\Shaders\\DefaultShader.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("NormalMapDefault", ".\\Resources\\Shaders\\DefaultShader.vert", ".\\Resources\\Shaders\\DefaultShaderNormalMap.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("SkeletNormalMapDefault", ".\\Resources\\Shaders\\SkeletalShader.vert", ".\\Resources\\Shaders\\DefaultShaderNormalMap.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		CreateShader("Wireframe", ".\\Resources\\Shaders\\WireframeShader.vert", ".\\Resources\\Shaders\\WireframeShader.frag");
		CreateShader("Skybox", ".\\Resources\\Shaders\\SkyboxShader.vert", ".\\Resources\\Shaders\\SkyboxShader.frag");
		CreateShader("Particle", ".\\Resources\\Shaders\\BillboardShader.vert", ".\\Resources\\Shaders\\ParticleShader.frag");

		Shader SkeletalShadow(".\\Resources\\Shaders\\SkeletalShadow.vert", ".\\Resources\\Shaders\\SkeletalShadow.frag");
		m_shaders.emplace("SkeletalShadow", std::make_shared<Shader>(SkeletalShadow));

		Shader skeletal(".\\Resources\\Shaders\\SkeletalShader.vert", ".\\Resources\\Shaders\\SkeletalShader.frag");
		m_shaders.emplace("Skeletal", std::make_shared<Shader>(skeletal));

		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->CreateDefaultMaterial(this);
		m_materials.emplace("Default", material);

		LoadObjInModel("Cube", ".\\Resources\\Models\\cube.obj");
		LoadObjInModel("Capsule", ".\\Resources\\Models\\capsule.obj");
		LoadObjInModel("Quad", ".\\Resources\\Models\\quad.obj");
		LoadObjInModel("Skybox", ".\\Resources\\Models\\skybox.obj");
		LoadObjInModel("Sphere", ".\\Resources\\Models\\sphere.obj");
	}

	ResourcesManager::~ResourcesManager()
	{
		ZoneScoped
		m_task->EndTaskSystem();

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

	void ResourcesManager::CreateNewMaterial(const std::string& path)
	{
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->CreateDefaultMaterial(this);
		material->SaveMaterial(path, this);
		m_materials.emplace(path, material);
	}

	void ResourcesManager::ReplaceMaterial(const std::string& path, const std::string& newPath)
	{
		auto it = m_materials.find(path);
		if (it != m_materials.end())
		{
			if(!it->second.use_count() > 1)
				it->second->UpdateNameMaterial(newPath);
			std::swap(m_materials[newPath], it->second);
			m_materials.erase(it);
		}
	}

	std::shared_ptr<Resources::Material> ResourcesManager::LoadBMatMaterial(const std::string& string)
	{
		std::shared_ptr<Resources::Material> material = std::make_shared<Resources::Material>();
		if (material->LoadMaterialFromJSON(string, this))
		{
			m_materials.emplace(string, material);
			return material;
		}
		return nullptr;
	}

	void ResourcesManager::CreateNewShader(const std::string& path)
	{
		auto shader = CreateShader(path.c_str(), ".\\Resources\\Shaders\\DefaultShader.vert", 
									".\\Resources\\Shaders\\DefaultShader.frag",
									Resources::Shader::EShaderHeaderType::LIGHT);

		shader->SaveShader(path);
	}

	void ResourcesManager::ReplaceShader(const std::string& path, const std::string& newPath)
	{
		auto it = m_shaders.find(path);
		if (it != m_shaders.end())
		{
			if(it->second.use_count() > 1)
				UpdateShaderPathInMaterials(it->second);
			std::swap(m_shaders[newPath], it->second);
			m_shaders.erase(it);
		}
	}

	void ResourcesManager::UpdateShaderPathInMaterials(std::shared_ptr<Resources::Shader> shader)
	{
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			if ((*it).second->shader == shader)
				(*it).second->SaveMaterial(it->first, this);
		}
	}

	std::shared_ptr<Resources::Shader> ResourcesManager::LoadBShaderShader(const std::string& string)
	{
		std::shared_ptr<Resources::Shader> shader = std::make_shared<Resources::Shader>();
		if (shader->LoadShader(string))
		{
			m_shaders.emplace(string, shader);
			return shader;
		}
		return nullptr;
	}

	Object3DInfo ResourcesManager::Load3DObject(const char* fileName, const bool graphInMulti)
	{
		std::string Name = fileName;
		Object3DInfo info{};

		if (m_scenes.count(Name) == 0)
			if (LoadAssimpScene(fileName, info, graphInMulti))
				return info;
		return info;
	}

	const aiScene* ResourcesManager::LoadSceneFromImporter(Assimp::Importer& importer,const char* fileName)
	{
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate // load the 3DObject with only triangle
			| aiProcess_JoinIdenticalVertices // join all vertices wich are the same for use indices for draw
			| aiProcess_SplitLargeMeshes
			| aiProcess_SortByPType
			| aiProcess_ValidateDataStructure // validate the scene data
			| aiProcess_CalcTangentSpace // calculate the tangent
			| aiProcess_GenBoundingBoxes // generate the AABB of the meshs aiProcess_Gen
			| aiProcess_GenNormals
			| aiProcess_LimitBoneWeights
		);

		return scene;
	}

	bool ResourcesManager::LoadAssimpScene(const char* fileName, Object3DInfo& info, const bool graphInMulti)
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
			LoadMeshsSceneInSingleMesh(importer, scene, directoryFile, info);
			LoadSceneResources(importer, scene, Name, directoryFile, graphInMulti);
		}
		else
		{
			if (scene->HasAnimations())
			{
				LoadAnimation(scene, directoryFile, fileName);
			}

			if (scene->mNumMeshes > 0)
			{
				LoadMeshsScene(importer, scene, directoryFile, info);
				LoadSceneResources(importer, scene, Name, directoryFile, graphInMulti);
			}
		}

		return true;
	}

	void ResourcesManager::LoadMeshsScene(	std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, 
											const std::string& directory, Object3DInfo& info)
	{
		unsigned int numBones{ 0 };

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
			std::shared_ptr<Model> model = std::make_shared<Model>();

			std::string name = directory + mesh->mName.data;

			unsigned int numberOfSameKey{ LoadMeshsSceneCheckModelIsLoaded(modelData, model, name) };

			info.modelsName.push_back(name);

			modelData->model = model;
			modelData->SetArrays(scene, i);

			m_task->AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, 0, 0);
			std::shared_ptr<unorderedmapBonesIndex> bonesIndex = std::make_shared<unorderedmapBonesIndex>();
			if (mesh->HasBones())
				LoadAnimationaiMesh(modelData, scene, mesh,
					name, numBones, 0, bonesIndex);

			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(importer, scene, mesh, directory, info, numberOfSameKey);
			}
		}
	}

	unsigned int ResourcesManager::LoadMeshsSceneCheckModelIsLoaded(std::shared_ptr<ModelData>& currModelData, 
																	std::shared_ptr<Model>& currModel, 
																	std::string& nameMesh)
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
			nameMesh = name;
		}
		else
		{
			currModelData->ModelName = name;
			m_modelsToLink.push_back(currModelData);
			m_models.emplace(name, currModel);
			nameMesh = name;
		}
		return numberOfSameKey;
	}

	void ResourcesManager::LoadMeshsSceneInSingleMesh(	std::shared_ptr<Loader::ImporterData>& importer, const aiScene* scene, 
														const std::string& directory, Object3DInfo& info)
	{
		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		unsigned int indexLastMesh{ 0 };

		modelData->model = model;
		m_models.emplace(directory + scene->mMeshes[0]->mName.data, model);

		info.modelsName.push_back(directory + scene->mMeshes[0]->mName.data);

		m_modelsToLink.push_back(modelData);
		modelData->ModelName = directory + scene->mMeshes[0]->mName.data;

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			modelData->SetArrays(scene, i);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			m_task->AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, i, indexLastMesh);

			indexLastMesh += mesh->mNumVertices;

			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(importer, scene, mesh, directory, info);
			}
		}
	}

	void ResourcesManager::LoadAnimationaiMesh(std::shared_ptr<ModelData>& modelData, const aiScene* scene, 
													aiMesh* mesh, const std::string& directory
												, unsigned int& numBones,  const unsigned int& numVertices, 
												std::shared_ptr<unorderedmapBonesIndex>& bonesIndex)
	{
		for (auto i{ 0 }; i < mesh->mNumBones; i++)
		{
			unsigned int currBoneIndex{ 0 };
			std::string nameBone = mesh->mBones[i]->mName.data;
			aiBone* currBone = mesh->mBones[i];
			{
				currBoneIndex = numBones;
				Animation::BoneData boneData;
				boneData.boneIndex = numBones;
				aiVector3D pos;
				aiQuaternion rot;
				aiVector3D sca;
				currBone->mOffsetMatrix.Decompose(sca, rot, pos);
				boneData.offsetMesh = Core::Datastructure::Transform({ pos.x, pos.y, pos.z }, 
																	{ rot.w, rot.x, rot.y, rot.z }, 
																	{ sca.x, sca.y, sca.z }).GetLocalTrs();
				bonesIndex->emplace(nameBone, boneData);
				numBones++;
			}
		}

		if (modelData->modelAnimationData.size() - numVertices == mesh->mNumVertices)
			LoadBonesHierarchy(scene, directory, numBones, bonesIndex);
	}

	void ResourcesManager::LoadBonesHierarchy(const aiScene* scene, const std::string& directory
												, const unsigned int& numBones
												, const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex)
	{
		const aiNode* firstBoneNode{ FindFirstBoneNode(scene->mRootNode, bonesIndex)};

		if (firstBoneNode == nullptr)
			return;
		
		std::shared_ptr<Core::Animation::BoneTree> tree = std::make_shared<Core::Animation::BoneTree>();
		aiVector3D pos;
		aiQuaternion rot;
		aiVector3D sca;
		firstBoneNode->mParent->mTransformation.Decompose(sca, rot, pos);

		constexpr Core::Maths::Mat<4,4> i{ i.Identity() };

		Core::Datastructure::Transform t{};
		
		tree->rootBone = std::make_shared<Core::Animation::Bone>();
		tree->rootBone->InitBone(firstBoneNode->mParent, bonesIndex, t);
		tree->numBone = numBones;

		m_BoneHierarchies.emplace(directory, tree);
	}

	const aiNode* ResourcesManager::FindFirstBoneNode(const aiNode* node, const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex)
	{
		if (bonesIndex->count(node->mName.data) > 0)
			return node;

		for (unsigned int i{ 0 }; i < node->mNumChildren; i++)
		{
			const aiNode* child = FindFirstBoneNode(node->mChildren[i], bonesIndex);
			if (child != nullptr)
				return child;
		}

		return nullptr;
	}

	std::shared_ptr<Core::Animation::Animation> ResourcesManager::LoadAsAnAnimation(const std::string& fileName)
	{
		if (m_animations.count(fileName) > 0)
			return m_animations[fileName];

		std::shared_ptr<ImporterData> importer = std::make_shared<ImporterData>();
		const aiScene* scene = LoadSceneFromImporter(importer->importer, fileName.c_str());

		if (!scene)
		{
			return nullptr;
		}

		if (!scene->HasAnimations())
			return nullptr;

		aiAnimation* anim = scene->mAnimations[0];

		std::shared_ptr<Core::Animation::Animation> animation = std::make_shared<Core::Animation::Animation>();

		animation->initAnimation(anim);

		m_animations.emplace(fileName, animation);

		return animation;
	}

	void ResourcesManager::LoadAnimation(const aiScene* scene, const std::string& directory, const std::string& fileName)
	{
		for (unsigned int i{ 0 }; i < scene->mNumAnimations; i++)
		{
			aiAnimation* anim = scene->mAnimations[i];

			std::shared_ptr<Core::Animation::Animation> animation = std::make_shared<Core::Animation::Animation>();

			animation->initAnimation(anim);
			if(scene->mNumAnimations == 1)
				m_animations.emplace(fileName, animation);
			else	
				m_animations.emplace(directory + anim->mName.data, animation);
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

		m_task->AddTask(&Resources::ModelData::LoadaiMeshModel, modelData.get(), mesh, importer, 0, 0);

		std::shared_ptr<Object3DGraph> sceneData = std::make_shared<Object3DGraph>();
	}

	void ResourcesManager::LoadaiMeshMaterial(std::shared_ptr<Loader::ImporterData> importer, 
												const aiScene* scene, aiMesh* mesh, 
												const std::string& directory, Object3DInfo& info, const int numberOfSameKey)
	{	
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
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

		info.materialsName.push_back(keyMaterial);

		m_materials.emplace(keyMaterial, materialOut);

		m_materialsGetUniform.push_back(materialOut);
		materialOut->IsSkeletal = mesh->HasBones();
		m_task->AddTask(&Resources::Material::LoadMaterialFromaiMaterial, materialOut, mat, directory, this, importer);

	}

	void ResourcesManager::LoadTexture(const std::string& keyName, std::shared_ptr<Texture>& texture)
	{
		if (m_textures.count(keyName) > 0)
		{
			texture = m_textures[keyName];
			return;
		}

		std::shared_ptr<TextureData> textureData = std::make_shared<TextureData>();

		if (!texture)
			texture = std::make_shared<Texture>();

		m_textures.emplace(keyName, texture);

		texture->name = keyName;
		textureData->nameTexture = keyName;
		PushTextureToLink(textureData);
		textureData->textureptr = texture;

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
				m_task->AddTask(&Resources::Object3DGraph::SceneLoad, sceneData.get(), importer,scene, rootNode, directory, true);
			else
				sceneData->SceneLoad(importer, scene, rootNode, directory, true);
			
		}
		else
		{
			if (inMultiThread)
				m_task->AddTask(&Resources::Object3DGraph::SceneLoad, sceneData.get(), importer, scene, rootNode, directory, false);
			else
				sceneData->SceneLoad(importer, scene, rootNode, directory, false);
		}

		m_scenes.emplace(fileName, sceneData);
	}

	void ResourcesManager::LinkAllTextureToOpenGl()
	{
		ZoneScoped
		for (auto it = m_texturesToLink.begin(); 
				it != m_texturesToLink.end();)
		{
			switch ((*it)->stateTexture)
			{
				case EOpenGLLinkState::LOADPROBLEM:
					m_textures.erase((*it)->nameTexture);
					it = m_texturesToLink.erase(it);
					break;
				case EOpenGLLinkState::CANTLINK:
					if ((*it)->ID == 0)
					{
						glGenTextures(1, &(*it)->textureptr->texture);
						(*it)->ID = (*it)->textureptr->texture;
						m_task->AddTask(&Resources::TextureData::CreateTextureFromImage, (*it), (*it)->nameTexture.c_str(), this, true);
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
		ZoneScoped
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
		ZoneScoped
		for (auto it = m_importerToDelete.begin();
			it != m_importerToDelete.end();)
		{
			if ((*it)->maxUseOfImporter == 0)
				it = m_importerToDelete.erase(it);
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

	std::shared_ptr<Core::Audio::AudioClip> ResourcesManager::CreateAudioClip(const std::string& audioClipPath, FMOD::System* fmodSystem)
	{
		if (m_audioClips[audioClipPath] == nullptr)
			m_audioClips[audioClipPath] = std::make_shared<Core::Audio::AudioClip>(audioClipPath, fmodSystem);

		return m_audioClips[audioClipPath];
	}

	std::shared_ptr<Shader> ResourcesManager::CreateShader(const char* shaderName, const char* vertexFilePath, const char* fragmentFilePath, Shader::EShaderHeaderType header)
	{
		ZoneScoped
		if (m_shaders.find(shaderName) != m_shaders.end())
			return m_shaders[shaderName];

		Shader newShader(vertexFilePath, fragmentFilePath, header);
		std::shared_ptr<Shader> newPtr = std::make_shared<Shader>(newShader);
		m_shaders.emplace(shaderName, newPtr);

		return newPtr;
	}

	void ResourcesManager::ShaderUpdate()
	{
		ZoneScoped
		for (unorderedmapShader::iterator itshader = m_shaders.begin();
			itshader != m_shaders.end(); ++itshader)
		{
			ZoneScopedN("ShaderUpdateLoop")
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

	std::string ResourcesManager::FindShaderFromShared(std::shared_ptr<Resources::Shader> shaderPTR)
	{
		std::string nameShader = "nothing";
		for (auto shader : m_shaders)
		{
			if (shader.second == shaderPTR)
			{
				nameShader = shader.first;
				break;
			}
		}

		return nameShader;
	}

	std::string ResourcesManager::FindMaterialFromShared(std::shared_ptr<Resources::Material> materialPTR)
	{
		std::string nameMaterial = "nothing";
		for (auto material : m_materials)
		{
			if (material.second == materialPTR)
			{
				nameMaterial = material.first;
				break;
			}
		}

		return nameMaterial;
	}

	std::string ResourcesManager::FindTextureFromShared(std::shared_ptr<Resources::Texture> texturePTR)
	{
		std::string nameTexture = "nothing";
		for (auto texture : m_textures)
		{
			if (texture.second == texturePTR)
			{
				nameTexture = texture.first;
				break;
			}
		}

		return nameTexture;
	}

	void ResourcesManager::SaveMaterial()
	{
		for (auto mat : m_materials)
		{
			if (mat.first.find(".bmat") != std::string::npos)
			{
				mat.second->SaveMaterial(mat.first, this);
			}
		}
	}

	void ResourcesManager::UpdateDeletedShaderMaterials(const std::shared_ptr<Resources::Shader>& shaderDeleted)
	{
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it)
		{
			if ((*it).second->shader == shaderDeleted)
				(*it).second->shader = m_shaders["DefaultNoTexture"];
		}
	}

	void ResourcesManager::DeleteSpecialMaterialShader(const std::string& path)
	{
		if (path.find(".bmat") != std::string::npos)
		{
			m_materials[path]->IsDelete = true;
			m_materials.erase(path);
		}
		else if (path.find(".bshader") != std::string::npos)
		{
			UpdateDeletedShaderMaterials(m_shaders[path]);
			m_shaders.erase(path);
		}
	}
}
