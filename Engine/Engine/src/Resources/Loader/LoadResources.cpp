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


namespace Resources::Loader
{
	
	void LoadResourcesIRenderable(Mesh* renderObject, const char* fileName)
	{
		std::string file = fileName;
		const aiScene* scene = aiImportFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if (!scene)
		{
			std::cout << "could not load file" << std::endl;
			return;
		}

		file.find_last_of("/");

		if (file.find(".obj"))
		{
			LoadSingleMeshResourcesIRenderable(renderObject, scene);
		}
		else
			std::cout << "c pas " << std::endl;


	}

	struct vertex
	{
		Core::Maths::Vec3 Position;
		Core::Maths::Vec2 UV;
		Core::Maths::Vec3 Normal;
	};

	void LoadSingleMeshResourcesIRenderable(Mesh* renderObject, const aiScene* scene)
	{
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> textures;
		vertex v;

		std::cout << scene->mNumMeshes << std::endl;
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[0];

			aiVector3D Zeor3D{ 0.f,0.f,0.f };

			for (int j = 0; j < mesh->mNumVertices; j++)
			{
				// get the position of the current vertice
				aiVector3D* Pos = &mesh->mVertices[j];
				// get the UV of the current vertice
				aiVector3D* UV = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][j]) : &Zeor3D;
				// get the Normal of the current vertice
				aiVector3D* Normal = &(mesh->mNormals[j]);

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
					indices.push_back(face.mIndices[iid]);
				}
			}

			renderObject->m_texture = LoadMaterialResourcesIRenderable(scene, mesh);
			std::cout << mesh->mNumVertices << std::endl;

		}

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		renderObject->m_VAO = VAO;

		renderObject->m_vertexCount = indices.size();
		std::cout << indices.size() << std::endl;
		glBindVertexArray(0);
		aiReleaseImport(scene);
	}

	unsigned int LoadMaterialResourcesIRenderable(const aiScene* scene, aiMesh* mesh)
	{
		if (!scene->HasMaterials())
		{
			return {};
		}
		std::vector<unsigned int> vec;
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fullPath = "Resources/Dog/";
				fullPath += path.data;
				std::cout << "Texture file path " << fullPath << std::endl;
				return Renderer::CreateTextureFromImage(fullPath.c_str(), true);
				/*std::string FullPath = Dir + "/" + Path.data;
				m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());*/
			}
		}

		return {};
	}
}
