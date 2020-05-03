#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "LoadResources.h"
#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "Assimp/Importer.hpp"
#include "Assimp/postprocess.h"
#include "Assimp/material.h"
#include "Assimp/texture.h"
#include "Assimp/RemoveComments.h"

#include "Object.hpp"
#include "Model.h"
#include "Texture.h"
#include "TextureData.h"
#include "Object3DGraph.h"
#include "BoneData.h"
#include "Mat.hpp"

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

		Shader normalMapShader("./Resources/Shaders/DefaultShader.vert", "./Resources/Shaders/DefaultShaderNormalMap.frag", Resources::Shader::EShaderHeaderType::LIGHT);
		m_shaders.emplace("NormalMapDefault", std::make_shared<Shader>(normalMapShader));

		Shader wireframeShader("./Resources/Shaders/WireframeShader.vert", "./Resources/Shaders/WireframeShader.frag");
		m_shaders.emplace("Wireframe", std::make_shared<Shader>(wireframeShader));

		Shader noTexture("./Resources/Shaders/NoTexture.vert", "./Resources/Shaders/NoTexture.frag");
		m_shaders.emplace("NoTexture", std::make_shared<Shader>(noTexture));

		Shader skeletal("./Resources/Shaders/SkeletalShader.vert", "./Resources/Shaders/SkeletalShader.frag");
		m_shaders.emplace("Skeletal", std::make_shared<Shader>(skeletal));

		LoadObjInModel("Cube","Resources/Models/cube.obj");
		LoadObjInModel("Capsule","Resources/Models/capsule.obj");
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

		Assimp::Importer importer;

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		//aiImportFile
		const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate // load the 3DObject with only triangle
											| aiProcess_GenSmoothNormals 
											| aiProcess_JoinIdenticalVertices // join all vertices wich are the same for use indices for draw
											| aiProcess_SplitLargeMeshes 
											| aiProcess_SortByPType 
											| aiProcess_ValidateDataStructure // validate the scene data
											| aiProcess_CalcTangentSpace // calculate the tangent
											| aiProcess_GenBoundingBoxes // generate the AABB of the meshs
											);
		if (!scene)
		{
			return false;
		}

		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		if (Name.find(".obj") != std::string::npos)
		{
			LoadMeshsSceneInSingleMesh(scene, directoryFile);
			Load3DObjectGraph(scene, Name, directoryFile);
		}
		else //if (Name.find(".fbx") != std::string::npos)
		{
			if (scene->mNumMeshes > 0)
			{
				bool isSkeletal{ false };
				int firstMeshWithBones{ 0 };

				if (scene->HasAnimations())
				{
					LoadAnimation(scene, directoryFile);
				}

				for (unsigned int i = 0; i < scene->mNumMeshes; i++)
				{
					if (scene->mMeshes[i]->HasBones())
					{
						isSkeletal = true;
						firstMeshWithBones = i;
						break;
					}

				}
				if (isSkeletal)
				{
					LoadMeshsSceneInSingleMesh(scene, directoryFile, isSkeletal,firstMeshWithBones);
					Load3DObjectGraph(scene, Name, directoryFile, firstMeshWithBones, isSkeletal);
				}
				else
				{
					LoadMeshsScene(scene, directoryFile);
					Load3DObjectGraph(scene, Name, directoryFile);
				}
			}
		}

		//aiReleaseImport(scene);
		importer.FreeScene();
		return true;
	}

	void ResourcesManager::LoadMeshsScene(const aiScene* scene, const std::string& directory)
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
			indexLastMesh = static_cast<unsigned int>(modelData->vertices.size());
			lastNumIndices = static_cast<unsigned int>(modelData->indices.size());
			modelData->LoadaiMeshModel(mesh);

			//modelData->LoadVertices(mesh);
			//modelData->LoadIndices(mesh);

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

	void ResourcesManager::LoadMeshsSceneInSingleMesh(const aiScene* scene, const std::string& directory, const bool& isSkeletal, const int& firstMeshWithBones)
	{
		if (m_models.count(directory + scene->mMeshes[firstMeshWithBones]->mName.data) > 0)
			return;

		std::shared_ptr<ModelData> skeletalModelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		unsigned int indexLastMesh{ 0 };
		unsigned int numBones{ 0 };

		skeletalModelData->model = model;
		m_models.emplace(directory + scene->mMeshes[firstMeshWithBones]->mName.data, model);
		m_modelsToLink.push_back(skeletalModelData);
		std::shared_ptr<unorderedmapBonesIndex> bonesIndex; 

		if (isSkeletal)
		{
			bonesIndex = std::make_shared<unorderedmapBonesIndex>();
			m_skeletalIndex.emplace(directory + scene->mMeshes[firstMeshWithBones]->mName.data, bonesIndex);
			skeletalModelData->SetArray(scene, isSkeletal);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			if (isSkeletal)
			{
				if (mesh->HasBones())
				{
					LoadAnimationaiMesh(skeletalModelData, scene, mesh, 
										directory + scene->mMeshes[firstMeshWithBones]->mName.data, 
										numBones, indexLastMesh, bonesIndex);
					skeletalModelData->LoadaiMeshModel(mesh, indexLastMesh);
					indexLastMesh += mesh->mNumVertices;
				}
			}
			else
			{
				skeletalModelData->LoadaiMeshModel(mesh, indexLastMesh);
				indexLastMesh += mesh->mNumVertices;
			}
			//modelData->LoadVertices(mesh);
			//modelData->LoadIndices(mesh, indexLastMesh);

			if (scene->HasMaterials())
			{
				LoadaiMeshMaterial(scene, mesh, directory);
			}
		}
		skeletalModelData->stateVAO = EOpenGLLinkState::CANLINK;
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
			if (bonesIndex->count(nameBone) <= 0)
			{
				currBoneIndex = numBones;
				Animation::BoneData boneData;
				boneData.boneIndex = numBones;
				aiVector3D pos;
				aiQuaternion rot;
				aiVector3D sca;
				currBone->mOffsetMatrix.Decompose(sca, rot, pos);
				boneData.offsetMesh = Core::Datastructure::Transform({ pos.x, pos.y, pos.z }, { rot.w, rot.x, rot.y, rot.z }, { sca.x, sca.y, sca.z }).GetLocalTrs();
				bonesIndex->emplace(nameBone, boneData);
				numBones++;
			}
			else
				currBoneIndex = bonesIndex->operator[](nameBone).boneIndex;

			modelData->LoadAnimationVertexData(mesh, currBoneIndex, currBone, numVertices);
		}

		if (modelData->modelAnimationData.size() - numVertices == mesh->mNumVertices)
			LoadBonesHierarchy(scene, directory, numBones, bonesIndex);
	}

	void ResourcesManager::LoadBonesHierarchy(const aiScene* scene, const std::string& directory
												, const unsigned int& numBones
												, const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex)
	{
		Core::Maths::Mat4 mat4;
		mat4(0, 0) = 1;
		mat4(1, 1) = 1;
		mat4(2, 2) = 1;
		mat4(3, 3) = 1;
		const aiNode* firstBoneNode{ FindFirstBoneNode(scene->mRootNode, bonesIndex, mat4)};

		if (firstBoneNode == nullptr)
			return;
		
		std::shared_ptr<Core::Animation::BoneTree> tree = std::make_shared<Core::Animation::BoneTree>();
		aiVector3D pos;
		aiQuaternion rot;
		aiVector3D sca;
		firstBoneNode->mParent->mTransformation.Decompose(sca, rot, pos);
		//scene->mRootNode->mTransformation.Decompose(sca, rot, pos);
		tree->inverseGlobal = /*mat4*/Core::Datastructure::Transform(
								{ pos.x, pos.y, pos.z },
								{ rot.w, rot.x, rot.y, rot.z },
								{ sca.x, sca.y, sca.z }).GetLocalTrs().Inversed();

		constexpr Core::Maths::Mat<4,4> i{ i.Identity() };

		Core::Datastructure::Transform t{};
		
		tree->rootBone.InitBone(firstBoneNode->mParent, bonesIndex, t);
		tree->numBone = numBones;

		m_BoneHierarchies.emplace(directory, tree);
	}

	const aiNode* ResourcesManager::FindFirstBoneNode(const aiNode* node, const std::shared_ptr<unorderedmapBonesIndex>& bonesIndex, 
														Core::Maths::Mat4& mat)
	{
		aiVector3D pos;
		aiQuaternion rot;
		aiVector3D sca;
		node->mTransformation.Decompose(sca, rot, pos);

		if (bonesIndex->count(node->mName.data) > 0)
			return node;
		mat = mat * Core::Datastructure::Transform(
			{ pos.x, pos.y, pos.z },
			{ rot.w, rot.x, rot.y, rot.z },
			{ sca.x, sca.y, sca.z }).GetLocalTrs();


		for (unsigned int i{ 0 }; i < node->mNumChildren; i++)
		{
			const aiNode* child = FindFirstBoneNode(node->mChildren[i], bonesIndex, mat);
			if (child != nullptr)
				return child;
		}

		return nullptr;
	}

	void ResourcesManager::LoadAnimation(const aiScene* scene, const std::string& directory)
	{
		for (unsigned int i{ 0 }; i < scene->mNumAnimations; i++)
		{
			aiAnimation* anim = scene->mAnimations[i];

			std::shared_ptr<Core::Animation::Animation> animation = std::make_shared<Core::Animation::Animation>();

			animation->initAnimation(anim);

			m_animations.emplace(directory + anim->mName.data, animation);
		}
	}

	void ResourcesManager::LoadObjInModel(const std::string& name, const char* fileName)
	{
		std::string Name = fileName;

		Assimp::Importer importer;

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		//aiImportFile
		const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate // load the 3DObject with only triangle
			| aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices // join all vertices wich are the same for use indices for draw
			| aiProcess_SplitLargeMeshes
			| aiProcess_SortByPType
			| aiProcess_ValidateDataStructure // validate the scene data
			| aiProcess_CalcTangentSpace // calculate the tangent
			| aiProcess_GenBoundingBoxes // generate the AABB of the meshs
		);
		if (!scene)
		{
			return;
		}

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		if (m_models.count(name) > 0)
			return;

		modelData->model = model;
		m_models.emplace(name, model);
		m_modelsToLink.push_back(modelData);
			
		aiMesh* mesh = scene->mMeshes[0];


		modelData->LoadaiMeshModel(mesh);

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

	void ResourcesManager::Load3DObjectGraph(const aiScene* scene, const std::string& fileName, const std::string& directory
												, const unsigned int& indexFirstMesh ,const bool& isSkeletal)
	{
		std::shared_ptr<Object3DGraph> sceneData = std::make_shared<Object3DGraph>();

		aiNode* rootNode = scene->mRootNode;
		if (fileName.find(".obj") != std::string::npos)
		{
			sceneData->SceneLoad(scene, rootNode, directory, true);
		}
		else if (isSkeletal)
		{
			sceneData->haveSkeletal = true;
			sceneData->rootNodeScene.Skeletal3DObjectLoad(scene, rootNode, directory, indexFirstMesh);
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
					//it++;
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

	void ResourcesManager::ShaderUpdate()
	{
		for (unorderedmapShader::iterator itshader = m_shaders.begin();
			itshader != m_shaders.end(); ++itshader)
		{
			itshader->second->UseProgram();
			itshader->second->SendLights();
		}
	}
}
