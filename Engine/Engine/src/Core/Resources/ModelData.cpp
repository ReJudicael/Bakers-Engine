#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "ModelData.h"
#include "OffsetMesh.h"
#include "Model.h"

namespace Resources
{
	void ModelData::LoadIndices(aiMesh* mesh, const int increaseIndices)
	{
		unsigned int lastNumIndices = static_cast<unsigned int>(indices.size());

		for (unsigned int fid{ 0 }; fid < mesh->mNumFaces; fid++)
		{
			aiFace& face = mesh->mFaces[fid];
			for (int iid = 0; iid < 3; iid++)
			{
				// get the indices of the face who is cut in triangle
				indices.push_back(face.mIndices[iid] + increaseIndices);
			}
		}

		OffsetMesh offset;
		offset.count = static_cast<unsigned int>(indices.size()) - lastNumIndices;
		offset.beginIndices = lastNumIndices;
		offset.materialIndices = static_cast<unsigned int>(offsetsMesh.size());

		offsetsMesh.push_back(offset);
		model->offsetsMesh = offsetsMesh;
	}

	void ModelData::LoadVertices(aiMesh* mesh)
	{
		Vertex v;
		aiVector3D Zeor3D{ 0.f,0.f,0.f };

		haveTangent = mesh->HasTangentsAndBitangents();

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
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
			if (haveTangent)
			{
				// get the tangent of the current vertice
				aiVector3D* Tangent = &(mesh->mTangents[j]);
				v.Tangent = { Tangent->x, Tangent->y, Tangent->z };
			}

			vertices.push_back(v);
		}
	}


	void ModelData::CreateVAOModel()
	{
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
		if (haveTangent)
		{
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		}
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		indices.clear();
		vertices.clear();
		VAOModel = VAO;

		glBindVertexArray(0);

		stateVAO = EOpenGLLinkState::ISLINK;

		EmplaceInModel();
	}

	void ModelData::EmplaceInModel()
	{
		model->stateVAO = stateVAO;
		model->VAOModel = VAOModel;
	}
}