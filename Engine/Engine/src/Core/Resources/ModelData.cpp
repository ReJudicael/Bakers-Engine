#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "ModelData.h"
#include "OffsetMesh.h"
#include "Model.h"

namespace Resources
{
	void ModelData::LoadIndices(aiMesh* mesh, const int increaseIndices)
	{
		GLuint lastNumIndices = indices.size();

		for (int fid = 0; fid < mesh->mNumFaces; fid++)
		{
			aiFace& face = mesh->mFaces[fid];
			for (int iid = 0; iid < 3; iid++)
			{
				// get the indices of the face who is cut in triangle
				indices.push_back(face.mIndices[iid] + increaseIndices);
			}
		}

		OffsetMesh offset;
		offset.count = indices.size() - lastNumIndices;
		offset.beginIndices = lastNumIndices;
		offset.materialIndices = offsetsMesh.size();

		offsetsMesh.push_back(offset);
		model->offsetsMesh = offsetsMesh;
	}

	void ModelData::LoadVertices(aiMesh* mesh)
	{
		Vertex v;
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
	}

	void ModelData::EmplaceInModel()
	{
		model->stateVAO = stateVAO;
		model->VAOModel = VAOModel;
		//model->offsetsMesh = offsetsMesh;
	}
}