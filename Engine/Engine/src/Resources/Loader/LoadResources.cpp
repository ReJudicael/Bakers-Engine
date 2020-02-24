#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	struct vertex
	{
		Core::Maths::Vec3 Position;
		Core::Maths::Vec2 UV;
		Core::Maths::Vec3 Normal;
	};
	
	void ResourcesManager::LoadResourcesIRenderable(Mesh* renderObject, const char* fileName, Core::Datastructure::Object* rootObject)
	{
		std::string file = fileName;
		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
		if (!scene)
		{
			std::cout << "could not load file" << std::endl;
			return;
		}


		auto index = file.find_last_of("/");

		std::string str;
		str = file.substr(0, index + 1);

		if (file.find(".obj") != std::string::npos)
		{
			LoadSingleMeshResourcesIRenderable(renderObject, scene, str);
			if (rootObject != nullptr)
			{
				rootObject->AddComponent(renderObject);
				std::cout << "Component Add " << std::endl;
				return;
			}
			else
				return;
		}
		else if (file.find(".fbx") != std::string::npos)
		{
			std::cout << scene->mNumMeshes << std::endl;
 			LoadSceneResources(scene, str);
			std::cout << "c un fbx je tente des truc " << std::endl;
		}

		aiReleaseImport(scene);
	}

	void ResourcesManager::LoadResourcesIRenderable(const char* fileName, Core::Datastructure::Object* rootObject, const bool newObjectChild)
	{
		Mesh* mesh { new Mesh() };

		std::string file = fileName;
		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if (!scene)
		{
			std::cout << "could not load file" << std::endl;
			return;
		}

		std::cout << "file is loaded" << std::endl;

		auto index = file.find_last_of("/");

		std::string str;
		str = file.substr(0, index + 1);

		if (file.find(".obj") != std::string::npos)
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
		if (file.find(".fbx") != std::string::npos)
		{
			LoadSceneResources(scene, str);
			std::cout << "c un fbx je tente des truc " << std::endl;
		}
		aiReleaseImport(scene);

	}


	void ResourcesManager::LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene, 
							const std::string& directory)
	{
		std::vector<vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<unsigned int> textures;
		vertex v;

		unsigned int indexLastMesh = 0;
		unsigned int lastNumIndices = 0;

		//bool* materialPush = new bool[scene->mNumMaterials];

		std::cout << "number of mesh" << scene->mNumMeshes << std::endl;
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


			renderObject->AddMaterial(LoadMaterialResourcesIRenderable(scene, mesh, directory));

			OffsetMesh offset;
			offset.count = indices.size() - lastNumIndices - 1;
			offset.beginIndices = lastNumIndices;
			renderObject->AddOffsetMesh(offset);

			indexLastMesh = vertices.size();
			lastNumIndices = indices.size();
		}

		std::cout << "end load" << scene->mNumMeshes << std::endl;


		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, UV));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, Normal));
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		renderObject->m_VAO = VAO;

		renderObject->m_vertexCount = indices.size();
		std::cout << indices.size() << std::endl;
		glBindVertexArray(0);
		//delete[] materialPush;
	}

	Material ResourcesManager::LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh, 
								const std::string& directory)
	{
		if (!scene->HasMaterials())
		{
			std::cout << "y en a pas" << std::endl;
			return {};
		}
		std::vector<unsigned int> vec;
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		Material material;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			std::cout << "texture find " << std::endl;
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				std::cout << "Texture file path " << fullPath << std::endl;
				unsigned int texture = Renderer::CreateTextureFromImage(fullPath.c_str(), true);
				if (texture > 0)
					material.textures.push_back(texture);
			}
		}
		else if (mat->GetTextureCount(aiTextureType_EMISSION_COLOR) > 0)
		{
			std::cout << "texture find " << std::endl;
			aiString path;
			if (mat->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = directory + path.data;
				std::cout << "Texture file path " << fullPath << std::endl;
				unsigned int texture = Renderer::CreateTextureFromImage(fullPath.c_str(), true);
				if (texture > 0)
					material.textures.push_back(texture);
			}
		}
		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.diffuseColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.ambientColor = { color.r, color.g, color.b };
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.specularColor = { color.r, color.g, color.b };
		material.materialColor = { 1.0f, 1.0f, 1.0f };

		return material;
	}

	void ResourcesManager::LoadSceneResources(const aiScene* scene, const std::string& directory)
	{
		SceneData sceneData{};

		aiNode* rootNode = scene->mRootNode;

		/*aiVector3D position; 
		aiVector3D rotation; 
		aiVector3D scale; */
		RecursiveSceneLoad(scene, rootNode, sceneData.rootNodeScene, directory);
		/*if (rootNode->mNumMeshes > 0)
			sceneData.rootNodeScene.nameMesh = directory + scene->mMeshes[rootNode->mMeshes[0]]->mName.data;

		rootNode->mTransformation.Decompose(scale, rotation, position);

		sceneData.rootNodeScene.position =	{ position.x, position.y, position.z };
		sceneData.rootNodeScene.rotation =	{ rotation.x, rotation.y, rotation.z };
		sceneData.rootNodeScene.scale =		{ scale.x, scale.y, scale.z };

		for (int i{ 0 }; i < rootNode->mNumChildren; i++)
		{
			RecursiveSceneLoad(scene, rootNode, sceneData.rootNodeScene, directory);
		}*/
	}

	void ResourcesManager::RecursiveSceneLoad(const aiScene* scene, const aiNode* node, Node& currentNode, 
										const std::string& directory)
	{
		aiVector3D position;
		aiVector3D rotation;
		aiVector3D scale;

		if (node->mNumMeshes > 0)
			currentNode.nameMesh = directory + scene->mMeshes[node->mMeshes[0]]->mName.data;

		node->mTransformation.Decompose(scale, rotation, position);

		currentNode.position = { position.x, position.y, position.z };
		currentNode.rotation = { rotation.x, rotation.y, rotation.z };
		currentNode.scale = { scale.x, scale.y, scale.z };

		int numChildren = node->mNumChildren;
		if (numChildren > 0)
			currentNode.children.resize(numChildren);
		for (int i{ 0 }; i < node->mNumChildren; i++)
		{
			std::cout << "je suis un enfant" << std::endl;
			RecursiveSceneLoad(scene, node->mChildren[i], currentNode.children[i], directory);
		}
	}
}
