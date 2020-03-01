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
#include "SceneData.h"

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
		CreateProgram(gVertexShaderStr, gFragmentShaderStr, "Default");
	}

	ResourcesManager::~ResourcesManager()
	{

	}

	void ResourcesManager::LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject)
	{
		std::string Name = fileName;

		std::shared_ptr<SceneData> scene;
		if (m_scenes.count(fileName) == 0)
			if (!LoadAssimpScene(fileName, rootObject))
				return;

		if (m_scenes.count(Name) != 0)
			scene = m_scenes[Name];

		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		CreateScene(scene, rootObject);
	}

	bool ResourcesManager::LoadAssimpScene(const char* fileName,
		Core::Datastructure::Object* rootComponent)
	{
		std::string Name = fileName;

		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate 
											| aiProcess_GenSmoothNormals 
											| aiProcess_JoinIdenticalVertices 
											| aiProcess_SplitLargeMeshes 
											| aiProcess_SortByPType 
											/*| aiProcess_PreTransformVertices*/
											| aiProcess_ValidateDataStructure 
											/*| aiProcess_FindDegenerates*/);
		//const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_Quality);
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
			LoadSingleMeshResourcesIRenderable(scene, Name, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
		}
		else if (Name.find(".fbx") != std::string::npos)
		{
			LoadMeshResourcesIRenderable(scene, Name, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
			// std::cout "it's an Fbx" << std::endl;
		}

		aiReleaseImport(scene);

		return true;
	}

	void ResourcesManager::CreateScene(std::shared_ptr<SceneData> scene, Core::Datastructure::Object* rootObject)
	{
		RecurciveCreateScene(scene->rootNodeScene, rootObject);
	}

	void ResourcesManager::RecurciveCreateScene(const Node& node, Core::Datastructure::Object* Object)
	{
		if (node.nameMesh.find("nothing") == std::string::npos)
		{
			Mesh* mesh = { new Mesh() };

			if (m_models.count(node.nameMesh) > 0)
			{
				mesh->AddModel(m_models[node.nameMesh]);
				mesh->m_program = m_shaders["Default"];
				Object->AddComponent(mesh);
			}
		}

		Object->SetScale(node.scale);
		//Object->SetScale({0,0,0});
		Object->SetPos(node.position);
		Object->SetRot(node.rotation);

		for (auto i{ 0 }; i < node.children.size(); i++)
		{
			Core::Datastructure::Object* childObject{ Object->CreateChild({}) };
			RecurciveCreateScene(node.children[i], childObject);
		}
	}

	void ResourcesManager::LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject, const bool newObjectChild)
	{
		/*Mesh* mesh { new Mesh() };

		std::string Name = fileName;
		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if (!scene)
		{
			// std::cout "could not load file" << std::endl;
			return;
		}

		// std::cout "file is loaded" << std::endl;

		auto index = Name.find_last_of("/");

		std::string str;
		str = Name.substr(0, index + 1);

		if (Name.find(".obj") != std::string::npos)
		{
			LoadSingleMeshResourcesIRenderable(mesh, scene, str);
			if (newObjectChild)
			{
				Core::Datastructure::Object* object{ rootObject->CreateChild({}) };
				object->AddComponent(mesh);
				return;
			}
			else
				return;


		}
		if (Name.find(".fbx") != std::string::npos)
		{
			LoadSceneResources(scene, Name, str);
			// std::cout "it's an FBX " << std::endl;
		}
		aiReleaseImport(scene);*/

	}

	void ResourcesManager::LoadMeshResourcesIRenderable(const aiScene* scene,
		const std::string& fileName, const std::string& directory)
	{
		// std::cout "num mesh " << scene->mNumMeshes << std::endl;

		unsigned int indexLastMesh{ 0 };
		unsigned int lastNumIndices{ 0 };

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
			std::shared_ptr<Model> model = std::make_shared<Model>();


			int tmpint{ 0 };
			if (m_models.count(directory + mesh->mName.data) > 0)
			{
				tmpint = 1;
				std::string name = directory + mesh->mName.data + std::to_string(tmpint);
				while (m_models.count(name) > 0)
				{
					tmpint++;
					name = directory + mesh->mName.data + std::to_string(tmpint);
				}
				modelData->ModelName = name;
				m_modelsToLink.push_back(modelData);
				m_models.emplace(name, model);
			}
			else
			{
				modelData->ModelName = directory + mesh->mName.data;
				m_modelsToLink.push_back(modelData);
				m_models.emplace(directory + mesh->mName.data, model);
			}

			// std::cout modelData->ModelName << std::endl;

			modelData->model = model;
			indexLastMesh = modelData->vertices.size();
			lastNumIndices = modelData->indices.size();

			modelData->LoadVertices(mesh);
			modelData->LoadIndices(mesh);

			/*OffsetMesh offset;
			offset.count = modelData->indices.size() - lastNumIndices;
			offset.beginIndices = lastNumIndices;
			offset.materialIndices = 0;

			modelData->offsetsMesh.push_back(offset);*/

			modelData->materialsModel.resize(modelData->offsetsMesh.size());
			modelData->materialsModel[modelData->offsetsMesh.size() - 1] = std::make_shared<Material>();
			LoadMaterialResourcesIRenderable(scene, mesh, modelData->materialsModel[modelData->offsetsMesh.size() - 1], directory, tmpint);

			modelData->stateVAO = EOpenGLLinkState::CANLINK;
		}
	}

	void ResourcesManager::LoadSingleMeshResourcesIRenderable(const aiScene* scene,
		const std::string& fileName, const std::string& directory)
	{
		if (m_models.count(directory + scene->mMeshes[0]->mName.data) > 0)
			return;

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
		std::shared_ptr<Model> model = std::make_shared<Model>();

		unsigned int indexLastMesh = 0;

		modelData->materialsModel.resize(scene->mNumMeshes);
		modelData->model = model;
		m_models.emplace(directory + scene->mMeshes[0]->mName.data, model);

		m_modelsToLink.push_back(modelData);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			modelData->LoadVertices(mesh);

			modelData->LoadIndices(mesh, indexLastMesh);

			indexLastMesh = modelData->vertices.size();

			modelData->materialsModel[i] = std::make_shared<Material>();
			LoadMaterialResourcesIRenderable(scene, mesh, modelData->materialsModel[i], directory);
		}
		modelData->stateVAO = EOpenGLLinkState::CANLINK;
	}

	void ResourcesManager::LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, 
								std::shared_ptr<Material>& materialOut, const std::string& directory, const int meshNameCall)
	{
		if (!scene->HasMaterials())
		{
			// std::cout "no Material " << std::endl;
			return;
		}
		std::vector<unsigned int> vec;
		
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		Material material;
		std::string keyMaterial;
		if (meshNameCall > 0)
			keyMaterial = directory + mat->GetName().data + std::to_string(meshNameCall);
		else
			keyMaterial = directory + mat->GetName().data;

		if (m_materials.count(keyMaterial) > 0)
		{
			// std::cout "already know : "<< keyMaterial << std::endl;
			materialOut = m_materials[keyMaterial];
			return;
		}
		m_materials.emplace(keyMaterial, materialOut);

		materialOut->textures.resize(2);

		materialOut->textures[0] = std::make_shared<Texture>();
		materialOut->textures[1] = std::make_shared<Texture>();
		// std::cout "new Material " << keyMaterial << std::endl;
		LoadTextureMaterial(mat, materialOut->textures[0], aiTextureType_DIFFUSE, directory);
		LoadTextureMaterial(mat, materialOut->textures[1], aiTextureType_NORMALS, directory);

		// maybe load a normalMap

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materialOut->diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		materialOut->ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		materialOut->specularColor = { color.r, color.g, color.b };
		materialOut->materialColor = { 1.0f, 1.0f, 1.0f };

	}

	bool ResourcesManager::LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<Texture>& texture, 
												const aiTextureType& textureType ,const std::string& directory)
	{
		if (mat->GetTextureCount(textureType) > 0)
		{
			aiString path;
			if (mat->GetTexture(textureType, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				std::shared_ptr<TextureData> textureData = std::make_shared<TextureData>();

				// std::cout "texture here " << fullPath << std::endl;
				textureData->nameTexture = fullPath;

				if (m_textures.count(fullPath) > 0)
				{
					// std::cout "already know textures : " << std::endl;
					texture = m_textures[fullPath];
				}
				else
				{
					m_texturesToLink.push_back(textureData);
					textureData->textureptr = texture;
					textureData->CreateTextureFromImage(fullPath.c_str(), *this);
					//CreateTextureFromImage(fullPath.c_str(), textureData, true);
					m_textures.emplace(fullPath, texture);
				}
				return true;
			}
		}
		return false;
	}

	void ResourcesManager::LoadSceneResources(const aiScene* scene, const std::string& fileName, const std::string& directory)
	{
		std::shared_ptr<SceneData> sceneData = std::make_shared<SceneData>();

		aiNode* rootNode = scene->mRootNode;

		sceneData->rootNodeScene.RecursiveSceneLoad(scene, rootNode, directory);

		m_scenes.emplace(fileName, sceneData);
	}

	void ResourcesManager::LinkAllTextureToOpenGl()
	{

		for (auto it = m_texturesToLink.begin(); 
				it != m_texturesToLink.end();)
		{
			if ((*it)->stateTexture == EOpenGLLinkState::LOADPROBLEM)
			{
				it = m_texturesToLink.erase(it);
				continue;
			}
			if ((*it)->stateTexture == EOpenGLLinkState::CANTLINK)
			{
				it++;
				continue;
			}
			if ((*it)->stateTexture == EOpenGLLinkState::ISLINK)
			{
				it = m_texturesToLink.erase(it);
				continue;
			}
			
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*it)->width, (*it)->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*it)->data);
			glGenerateMipmap(GL_TEXTURE_2D);

			(*it)->stateTexture = EOpenGLLinkState::ISLINK;
			(*it)->texture = texture;

			(*it)->EmplaceInTexture();
			it = m_texturesToLink.erase(it);
		}
	}

	void ResourcesManager::LinkAllModelToOpenGl()
	{
		//// std::cout "Link " << m_modelsToLink.size() << std::endl;
		for (auto it = m_modelsToLink.begin();
			it != m_modelsToLink.end();)
		{
			if ((*it)->stateVAO == EOpenGLLinkState::LOADPROBLEM)
			{
				it = m_modelsToLink.erase(it);
				continue;
			}
			if ((*it)->stateVAO == EOpenGLLinkState::CANTLINK)
			{
				it++;
				continue;
			}
			if ((*it)->stateVAO == EOpenGLLinkState::ISLINK)
			{
				it = m_modelsToLink.erase(it);
				continue;
			}

			GLuint VAO, VBO, EBO;

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, (*it)->vertices.size() * sizeof(Vertex), &(*it)->vertices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*it)->indices.size() * sizeof(GLuint), &(*it)->indices[0], GL_STATIC_DRAW);

			(*it)->indices.clear();
			(*it)->vertices.clear();
			(*it)->VAOModel = VAO;

			glBindVertexArray(0);

			(*it)->stateVAO = EOpenGLLinkState::ISLINK;

			(*it)->EmplaceInModel();
			it = m_modelsToLink.erase(it);
		}
	}

	void	ResourcesManager::CreateProgram(const char* vertex, const char* fragment, const std::string& nameShader)
	{
		if (m_shaders.count(nameShader) > 0)
			return;

		// std::cout "Tried to create program" << std::endl;
		GLuint Program = glCreateProgram();

		GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
		std::vector<const char*> Sources;
		Sources.push_back("#version 330 core");
		Sources.push_back(vertex);
		glShaderSource(VertexShader, (GLsizei)Sources.size(), &Sources[0], nullptr);
		glCompileShader(VertexShader);
		GLint Compile;
		glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compile);
		if (Compile == GL_FALSE)
		{
			// std::cout "Vertex shader didn't load." << std::endl;
			return;
		}

		GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		std::vector<const char*> FragSources;
		FragSources.push_back("#version 330 core");
		FragSources.push_back(fragment);
		glShaderSource(FragmentShader, (GLsizei)FragSources.size(), &FragSources[0], nullptr);
		glCompileShader(FragmentShader);
		glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compile);
		if (Compile == GL_FALSE)
		{
			// std::cout "Fragment shader didn't load." << std::endl;
			return;
		}

		glAttachShader(Program, VertexShader);
		glAttachShader(Program, FragmentShader);

		glLinkProgram(Program);

		m_shaders.emplace(nameShader, std::make_shared<GLuint>(Program));

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		// std::cout "program load" << std::endl;
	}
}