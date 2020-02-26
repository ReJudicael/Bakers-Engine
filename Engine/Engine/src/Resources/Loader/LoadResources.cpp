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

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(uColorTexture, vUV);
	//oColor = vec4(vUV,0.0f,0.0f);
})GLSL";


namespace Resources::Loader
{

	ResourcesManager::ResourcesManager()
	{
		CreateProgram(gVertexShaderStr, gFragmentShaderStr, "Default");
	}

	void ResourcesManager::LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, Core::Datastructure::Object* rootObject)
	{
		std::string Name = fileName;

		std::shared_ptr<SceneData> scene;
		persperct = renderObject->m_projection;
		if (m_scenes.count(fileName) == 0)
			if (!LoadAssimpScene(renderObject, fileName, rootObject))
				return;

		if (m_scenes.count(Name) != 0)
			scene = m_scenes[Name];

		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		CreateScene(scene, rootObject);
	}

	bool ResourcesManager::LoadAssimpScene(Mesh* renderObject, const char* fileName,
		Core::Datastructure::Object* rootComponent)
	{
		std::string Name = fileName;

		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
		if (!scene)
		{
			std::cout << "could not load file" << std::endl;
			return false;
		}


		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		if (Name.find(".obj") != std::string::npos)
		{
			LoadSingleMeshResourcesIRenderable(renderObject, scene, Name, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
		}
		else if (Name.find(".fbx") != std::string::npos)
		{
			std::cout << scene->mNumMeshes << std::endl;
			LoadSceneResources(scene, Name, directoryFile);
			std::cout << "it's an Fbx" << std::endl;
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
				mesh->m_projection = persperct;
				mesh->m_program = m_shaders["Default"];
				Object->AddComponent(mesh);
			}
		}

		Object->SetScale(node.scale);
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
			std::cout << "could not load file" << std::endl;
			return;
		}

		std::cout << "file is loaded" << std::endl;

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
			std::cout << "it's an FBX " << std::endl;
		}
		aiReleaseImport(scene);*/

	}


	void ResourcesManager::LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene,
		const std::string& fileName, const std::string& directory)
	{

		std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<unsigned int> textures;
		Vertex v;

		unsigned int indexLastMesh = 0;
		unsigned int lastNumIndices = 0;

		modelData->materialsModel.resize(scene->mNumMeshes);

		m_models.emplace(directory + scene->mMeshes[0]->mName.data, modelData);

		m_modelsToLink.push_back(modelData);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			aiVector3D Zeor3D{ 0.f,0.f,0.f };

			for (int j = 0; j < mesh->mNumVertices; j++)
			{
				// get the position of the current vertice
				aiVector3D* Pos = &mesh->mVertices[j];
				// get the UV of the current vertice
				aiVector3D* UV = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &Zeor3D;
				// get the Normal of the current vertice
				aiVector3D* Normal = &(mesh->mNormals[j]);

				//std::cout << v.Position.x << std::endl;
				v.Position = { Pos->x, Pos->y, Pos->z };
				v.UV = { UV->x, UV->y };
				v.Normal = { Normal->x, Normal->y, Normal->z };

				modelData->vertices.push_back(v);

			}

			for (int fid = 0; fid < mesh->mNumFaces; fid++)
			{
				aiFace& face = mesh->mFaces[fid];
				for (int iid = 0; iid < 3; iid++)
				{
					// get the indices of the face who is cut in triangle
					modelData->indices.push_back(face.mIndices[iid] + indexLastMesh);
				}
			}

			modelData->materialsModel[i] = std::make_shared<Material>();
			LoadMaterialResourcesIRenderable(scene, mesh, modelData->materialsModel[i], directory);

			OffsetMesh offset;
			offset.count = modelData->indices.size() - lastNumIndices;
			offset.beginIndices = lastNumIndices;
			offset.materialIndices = i;

			indexLastMesh = modelData->vertices.size();
			lastNumIndices = modelData->indices.size();

			modelData->offsetsMesh.push_back(offset);
		}
		modelData->stateVAO = EOpenGLLinkState::CANLINK;
	}

	void ResourcesManager::LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, 
								std::shared_ptr<Material> materialOut, const std::string& directory)
	{
		if (!scene->HasMaterials())
		{
			std::cout << "no Material " << std::endl;
			return;
		}
		std::vector<unsigned int> vec;
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		Material material;

		std::string keyMaterial = directory + mat->GetName().data;

		if (m_materials.count(keyMaterial) > 0)
		{
			materialOut = m_materials[keyMaterial];
			return;
		}
		m_materials.emplace(directory + mat->GetName().data, materialOut);

		materialOut->textures.resize(1);

		materialOut->textures[0] = std::make_shared<TextureData>();
		LoadTextureMaterial(mat, materialOut->textures[0], aiTextureType_DIFFUSE, directory);


		// maybe load a normalMap

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materialOut->diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		materialOut->ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		materialOut->specularColor = { color.r, color.g, color.b };
		materialOut->materialColor = { 1.0f, 1.0f, 1.0f };

		//return material;
	}

	void ResourcesManager::LoadTextureMaterial(aiMaterial* mat, std::shared_ptr<TextureData> textureData, 
												const aiTextureType& textureType ,const std::string& directory)
	{
		if (mat->GetTextureCount(textureType) > 0)
		{
			aiString path;
			if (mat->GetTexture(textureType, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;

				if (m_textures.count(fullPath) != 0)
					textureData = m_textures[fullPath];
				else
					CreateTextureFromImage(fullPath.c_str(), textureData, true);
					m_textures.emplace(fullPath, textureData);
			}
		}
	}


	void ResourcesManager::LoadSceneResources(const aiScene* scene, const std::string& fileName, const std::string& directory)
	{
		std::shared_ptr<SceneData> sceneData = std::make_shared<SceneData>();

		aiNode* rootNode = scene->mRootNode;

		RecursiveSceneLoad(scene, rootNode, sceneData->rootNodeScene, directory);

		m_scenes.emplace(fileName, sceneData);
	}

	void ResourcesManager::RecursiveSceneLoad(const aiScene* scene, const aiNode* node, Node& currentNode,
		const std::string& directory)
	{
		aiVector3D position;
		aiVector3D rotation;
		aiVector3D scale;

		if (node->mNumMeshes > 0)
		{
			currentNode.nameMesh = directory + scene->mMeshes[node->mMeshes[0]]->mName.data;
			std::cout << "nameMesh " << currentNode.nameMesh << std::endl;
		}
		else
		{
			currentNode.nameMesh = "nothing";
		}

		node->mTransformation.Decompose(scale, rotation, position);

		currentNode.position = { position.x, position.y, position.z };
		currentNode.rotation = { rotation.x, rotation.y, rotation.z };
		currentNode.scale = { scale.x, scale.y, scale.z };

		int numChildren = node->mNumChildren;
		if (numChildren > 0)
			currentNode.children.resize(numChildren);
		for (int i{ 0 }; i < node->mNumChildren; i++)
			RecursiveSceneLoad(scene, node->mChildren[i], currentNode.children[i], directory);
	}

	void ResourcesManager::CreateTextureFromImage(const char* filename, std::shared_ptr<TextureData> textureData, bool shouldFlip)
	{
		std::string s = filename;

		if (m_textures.count(filename) > 0)
			textureData = m_textures[filename];

		std::cout << filename << std::endl;
		// load and generate the texture
		int nrChannels;
		stbi_set_flip_vertically_on_load(shouldFlip);
		textureData->data = stbi_load(s.c_str(), &textureData->width, &textureData->height, &nrChannels, 4);

		if (!textureData->data)
		{
			std::cout << "Failed to load " << filename << std::endl;
			stbi_image_free(textureData->data);
			textureData->stateTexture = EOpenGLLinkState::LOADPROBLEM;
			return;
		}
		textureData->stateTexture = EOpenGLLinkState::CANLINK;
		m_textures.emplace(filename, textureData);
		m_texturesToLink.push_back(textureData);
	}

	void ResourcesManager::LinkAllTextureToOpenGl()
	{

		for (std::list<std::shared_ptr<TextureData>>::iterator it = m_texturesToLink.begin(); 
				it != m_texturesToLink.end(); ++it)
		{
			if ((*it)->stateTexture == EOpenGLLinkState::LOADPROBLEM)
			{
				m_texturesToLink.erase(it);
			}
			if ((*it)->stateTexture == EOpenGLLinkState::CANTLINK)
				return;
			if ((*it)->stateTexture == EOpenGLLinkState::ISLINK)
				return;
			std::cout << "Link Texture" << std::endl;
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

			//m_texturesToLink.erase(it);
		}
	}

	void ResourcesManager::LinkAllModelToOpenGl()
	{
		//std::cout << "Link " << m_modelsToLink.size() << std::endl;
		for (std::list<std::shared_ptr<ModelData>>::iterator it = m_modelsToLink.begin();
			it != m_modelsToLink.end(); it++)
		{
			if ((*it)->stateVAO == EOpenGLLinkState::LOADPROBLEM)
			{
				m_modelsToLink.erase(it);
			}
			if ((*it)->stateVAO == EOpenGLLinkState::CANTLINK)
				return;
			if ((*it)->stateVAO == EOpenGLLinkState::ISLINK)
				return;

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
			//m_modelsToLink.erase(it);
		}
	}

	void	ResourcesManager::CreateProgram(const char* vertex, const char* fragment, const std::string& nameShader)
	{
		if (m_shaders.count(nameShader) > 0)
			return;

		std::cout << "Tried to create program" << std::endl;
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
			std::cout << "Vertex shader didn't load." << std::endl;
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
			std::cout << "Fragment shader didn't load." << std::endl;
			return;
		}

		glAttachShader(Program, VertexShader);
		glAttachShader(Program, FragmentShader);

		glLinkProgram(Program);

		m_shaders.emplace(nameShader, std::make_shared<GLuint>(Program));

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		std::cout << "program load" << std::endl;
	}
}
