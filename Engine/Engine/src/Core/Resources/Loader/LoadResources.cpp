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
		CreateProgram(gVertexShaderStr, gFragmentShaderStr, "Default");
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
			glDeleteProgram(*itshader->second);
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
											/*| aiProcess_PreTransformVertices*/
											| aiProcess_ValidateDataStructure 
											/*| aiProcess_FindDegenerates*/);
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
