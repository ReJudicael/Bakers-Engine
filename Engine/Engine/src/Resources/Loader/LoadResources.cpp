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

namespace Resources::Loader
{

	void ResourcesManager::LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, Core::Datastructure::Object* rootObject)
	{
		std::string Name = fileName;

		/*const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
		if (!scene)
		{
			std::cout << "could not load file" << std::endl;
			return;
		}*/

		std::shared_ptr<SceneData> scene;

		if (m_scenes.count(fileName) == 0)
			if (!LoadAssimpScene(renderObject, fileName, rootObject))
				return;

		if (m_scenes.count(Name) != 0)
			scene = m_scenes[Name];

		auto index = Name.find_last_of("/");

		std::string directoryFile;
		directoryFile = Name.substr(0, index + 1);

		std::cout << "coucou" << std::endl;
		CreateScene(scene, rootObject);
		/*if (Name.find(".obj") != std::string::npos)
		{
			LoadSingleMeshResourcesIRenderable(renderObject, scene, Name, directoryFile);
			LoadSceneResources(scene, Name, directoryFile);
			if (rootObject != nullptr)
			{
				rootObject->AddComponent(renderObject);
				std::cout << "Component Add " << std::endl;
				return;
			}
			else
				return;
		}
		else if (Name.find(".fbx") != std::string::npos)
		{
			std::cout << scene->mNumMeshes << std::endl;
			LoadSceneResources(scene, Name, directoryFile);
			std::cout << "it's an Fbx" << std::endl;
		}

		aiReleaseImport(scene);*/


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
		std::cout << "I am in" << std::endl;
		RecurciveCreateScene(scene->rootNodeScene, rootObject);

	}

	void ResourcesManager::RecurciveCreateScene(const Node& node, Core::Datastructure::Object* Object)
	{
		if (node.nameMesh.find("nothing") == std::string::npos)
		{
			std::cout << "coucou" << node.nameMesh << std::endl;
			Mesh* mesh = { new Mesh() };

			mesh->AddModel(m_models[node.nameMesh]);
			mesh->m_program = 1;
			Object->AddComponent(mesh);
		}

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


		std::cout << "number of mesh" << scene->mNumMeshes << std::endl;

		m_models.emplace(fileName, modelData);

		modelData->materialsModel.resize(scene->mNumMeshes);

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

				vertices.push_back(v);

			}

			for (int fid = 0; fid < mesh->mNumFaces; fid++)
			{
				aiFace& face = mesh->mFaces[fid];
				for (int iid = 0; iid < 3; iid++)
				{
					// get the indices of the face who is cut in triangle
					indices.push_back(face.mIndices[iid] + indexLastMesh);
				}
			}


			LoadMaterialResourcesIRenderable(scene, mesh, modelData->materialsModel[i], directory);

			OffsetMesh offset;
			offset.count = indices.size() - lastNumIndices - 1;
			offset.beginIndices = lastNumIndices;

			//renderObject->AddOffsetMesh(offset);

			indexLastMesh = vertices.size();
			lastNumIndices = indices.size();

			modelData->offsetsMesh.push_back(offset);
		}

		std::cout << "end load" << scene->mNumMeshes << std::endl;

		// To Do prepare the multithreading
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		renderObject->m_VAO = VAO;
		modelData->VAOModel = VAO;

		renderObject->m_vertexCount = indices.size();
		std::cout << indices.size() << std::endl;


		glBindVertexArray(0);
	}

	void ResourcesManager::LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, std::shared_ptr<Material> materialOut,
		const std::string& directory)
	{
		if (!scene->HasMaterials())
		{
			std::cout << "no Material " << std::endl;
			return;
		}

		std::vector<unsigned int> vec;
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::shared_ptr<Material> material = std::make_shared<Material>();

		std::string keyMaterial = directory + mat->GetName().data;

		if (m_materials.count(keyMaterial) > 0)
		{
			materialOut = m_materials[keyMaterial];
			return;
		}

		m_materials.emplace(directory + mat->GetName().data, material);
		materialOut = material;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			std::cout << "texture find " << std::endl;
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				std::cout << "Texture file path " << fullPath << std::endl;
				std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();
				material->textures.push_back(texture);

				if (m_textures.count(fullPath) != 0)
					texture = m_textures[fullPath];
				else
				{
					CreateTextureFromImage(fullPath.c_str(), texture,true);
					m_textures.emplace(fullPath, texture);
				}

				if (texture > 0)
					material->textures.push_back(texture);
			}
		}
		/*else if (mat->GetTextureCount(aiTextureType_EMISSION_COLOR) > 0)
		{
			std::cout << "texture find in Emission" << std::endl;
			aiString path;
			if (mat->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				std::cout << "Texture file path " << fullPath << std::endl;
				unsigned int texture = Renderer::CreateTextureFromImage(fullPath.c_str(), true);
				if (texture > 0)
					material.textures.push_back(texture);
			}
		}*/

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specularColor = { color.r, color.g, color.b };
		material->materialColor = { 1.0f, 1.0f, 1.0f };

		//return material;
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
			std::cout << "nothing " << currentNode.nameMesh << std::endl;
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


		std::cout << filename << std::endl;
		// load and generate the texture
		int nrChannels;
		stbi_set_flip_vertically_on_load(shouldFlip);
		textureData->data = stbi_load(s.c_str(), &textureData->width, &textureData->height, &nrChannels, 4);


		/*if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}*/
		if (!textureData->data)
		{
			std::cout << "Failed to load " << filename << std::endl;
			stbi_image_free(textureData->data);
		}

	}

	void ResourcesManager::linkAllTextureToOpenGl()
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
			
			std::cout << "Link" << std::endl;
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

			m_texturesToLink.erase(it);
		}
	}

	void ResourcesManager::linkAllModelToOpenGl()
	{
		for (std::list<std::shared_ptr<ModelData>>::iterator it = m_modelsToLink.begin();
			it != m_modelsToLink.end(); ++it)
		{
			if ((*it)->stateVAO == EOpenGLLinkState::LOADPROBLEM)
			{
				m_modelsToLink.erase(it);
			}
			if ((*it)->stateVAO == EOpenGLLinkState::CANTLINK)
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

			glBindVertexArray(0);

			(*it)->stateVAO = EOpenGLLinkState::ISLINK;
			m_modelsToLink.erase(it);
		}
	}
}
