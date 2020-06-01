#include "Assimp/cimport.h"
#include "Assimp/scene.h"
#include "ModelData.h"
#include "OffsetMesh.h"
#include "LoadResources.h"
#include "Model.h"
#include "LoadResources.h"

namespace Resources
{

	/*void ModelData::SetArray(const aiScene* scene, const bool& isSkeletal)
	{
		if (isSkeletal)
		{
			unsigned int numVertices{ 0 };
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				if(scene->mMeshes[i]->HasBones())
					numVertices += scene->mMeshes[i]->mNumVertices;
			}
			haveBones = true;
			modelAnimationData.resize(numVertices);
		}
	}*/

	void ModelData::SetArrays(const aiScene* scene, const unsigned int& index)
	{
		aiMesh* mesh = scene->mMeshes[index];
		auto indiceBegin = indices.size();
		vertices.resize(vertices.size() + mesh->mNumVertices);
		indices.resize(static_cast<int>(indices.size()) + mesh->mNumFaces * 3);

		OffsetMesh offset;
		offset.count = static_cast<unsigned int>(indices.size() - indiceBegin);
		offset.beginIndices = static_cast<GLuint>(indiceBegin);
		offset.materialIndices = static_cast<unsigned int>(offsetsMesh.size());

		offsetsMesh.push_back(offset);
		offsetMeshState.push_back(EOpenGLLinkState::CANTLINK);
		model->offsetsMesh = offsetsMesh;

		if (mesh->HasBones())
		{
			haveBones = true;
			modelAnimationData.resize(vertices.size());
			//offsetMeshState.resize(offsetMeshState.size() + mesh->mNumBones);
		}
	}

	void ModelData::LoadaiMeshModel(aiMesh* mesh, std::shared_ptr<Loader::ImporterData>& importer, 
									const unsigned int indexMesh, const int increaseIndices)
	{
		if (mesh == nullptr)
			stateVAO = EOpenGLLinkState::LOADPROBLEM;
		importer->maxUseOfImporter++;
		if (increaseIndices == 0)
			LoadaiMeshAABB(mesh);

		model->haveBones = haveBones;
		LoadVertices(mesh, increaseIndices);
		LoadIndices(mesh, increaseIndices, indexMesh);
		LoadAnimationVertexDatas(mesh, increaseIndices);

		offsetMeshState[indexMesh] = EOpenGLLinkState::CANLINK;
		importer->maxUseOfImporter--;
		
		for (auto i{ 0 }; i < offsetMeshState.size(); i++)
		{
			if (offsetMeshState[i] != EOpenGLLinkState::CANLINK)
				return;
		}
		stateVAO = EOpenGLLinkState::CANLINK;
	}

	void  ModelData::LoadaiMeshAABB(aiMesh* mesh)
	{
		aiVector3D AABBBox{ mesh->mAABB.mMin };
		min = { AABBBox.x, AABBBox.y, AABBBox.z };

		AABBBox = mesh->mAABB.mMax;
		max = { AABBBox.x, AABBBox.y, AABBBox.z };
	}

	void ModelData::LoadIndices(aiMesh* mesh, const int increaseIndices, const unsigned int indexMesh)
	{
		unsigned int index{ offsetsMesh[indexMesh].beginIndices };

		for (unsigned int fid{ 0 }; fid < mesh->mNumFaces; fid++)
		{
			aiFace& face = mesh->mFaces[fid];
			for (int iid = 0; iid < 3; iid++)
			{
				// get the indices of the face who is cut in triangle
				//indices.push_back(face.mIndices[iid] + increaseIndices);
				indices[index] = face.mIndices[iid] + increaseIndices;
				index++;
			}
		}
	}

	void ModelData::LoadVertices(aiMesh* mesh, const unsigned int increaseIndices)
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
			aiVector3D* Normal = mesh->HasNormals() ? &(mesh->mNormals[j]): &Zeor3D;

			v.Position = { Pos->x, Pos->y, Pos->z };
			v.UV = { UV->x, UV->y };
			v.Normal = { Normal->x, Normal->y, Normal->z };
			if (haveTangent)
			{
				// get the tangent of the current vertice
				aiVector3D* Tangent = &(mesh->mTangents[j]);
				v.Tangent = { Tangent->x, Tangent->y, Tangent->z };
			}

			vertices[static_cast<int>(increaseIndices + j)] = v;
		}
	}

	void ModelData::LoadAnimationVertexDatas(aiMesh* mesh, const unsigned int increaseIndices)
	{
		for (unsigned i{ 0 }; i < mesh->mNumBones; i++)
		{
			LoadAnimationVertexData(mesh, i, mesh->mBones[i], increaseIndices);
		}
	}

	void ModelData::LoadAnimationVertexData(aiMesh* mesh, const unsigned int boneIndex, 
											aiBone* currBone, const unsigned int numVertices)
	{
		for (unsigned int j{ 0 }; j < currBone->mNumWeights; j++)
		{
			unsigned int VertexID = numVertices + currBone->mWeights[j].mVertexId;
			modelAnimationData[VertexID].AddBoneData(boneIndex, currBone->mWeights[j].mWeight);
		}
	}

	void ModelData::CreateVAOModel()
	{
		GLuint VAO, VBO1, VBO2, EBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
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

		if (haveBones)
		{
			glGenBuffers(1, &VBO2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO2);
			glBufferData(GL_ARRAY_BUFFER, modelAnimationData.size() * sizeof(Animation::AnimationVertexData), 
											&modelAnimationData[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Animation::AnimationVertexData),
									(void*)offsetof(Animation::AnimationVertexData, boneIDs));
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Animation::AnimationVertexData), 
									(void*)offsetof(Animation::AnimationVertexData, weights));
		}

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		//indices.clear();
		//vertices.clear();
		VAOModel = VAO;

		glBindVertexArray(0);

		stateVAO = EOpenGLLinkState::ISLINK;

		EmplaceInModel();
	}

	void ModelData::EmplaceInModel()
	{
		model->indices = indices;
		model->vertices = vertices;
		model->max = max;
		model->min = min;
		model->stateVAO = stateVAO;
		model->VAOModel = VAOModel;
	}
}