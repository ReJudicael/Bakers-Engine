#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
 
#include "Vertex.h" 
#include "AnimationVertexData.h" 
#include "OpenGlLinkState.h" 
#include "CoreMinimal.h"


struct aiMesh;
struct aiBone;
struct aiScene;

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}
	struct Material;
	struct OffsetMesh;
	struct Model;
	namespace Loader
	{
		struct ImporterData;
	}

	/** 
	 * Class use for create a Model, 
	 * contains all the values use for link a Model to OpenGL
	 * and for animated Mesh
	 */
	struct ModelData
	{
		GLuint VAOModel{0};
		bool						haveTangent;
		std::vector<Vertex>			vertices;
		std::vector<GLuint>			indices;
		std::vector<OffsetMesh>		offsetsMesh;
		std::vector<EOpenGLLinkState> offsetMeshState;
		Core::Maths::Vec3			min;
		Core::Maths::Vec3			max;
		std::shared_ptr<Model>		model;
		bool											haveBones;
		std::vector<Animation::AnimationVertexData>		modelAnimationData;

		std::string ModelName;
		EOpenGLLinkState stateVAO;

		/**
		 * Load all the value of the aiMesh
		 * @param scene: The aiScene who is currently loaded
		 * @param index: the index of the aiMesh in the aiScene
		 */
		void SetArrays(const aiScene* scene, const unsigned int& index);

		/**
		 * Load all the value of the aiMesh
		 * @param mesh: The aiMesh who is currently loaded
		 * @param importer: the importer who currently have the scene of the aiMesh
		 * @param indexMesh: the index of the mesh if the current aiMesh have 
		 * the same name of an other mesh in the 3D Object
		 */
		void LoadaiMeshModel(aiMesh* mesh, std::shared_ptr<Loader::ImporterData>& importer, 
								const unsigned int indexMesh = 0, 
								const int increaseIndices = 0);
		
		/**
		 * Load the AABB of an aiMesh
		 * @param mesh: The aiMesh who is currently loaded
		 */
		void LoadaiMeshAABB(aiMesh* mesh);

		/**
		 * Load all the vertices of one aiMesh
		 * @param mesh: The aiMesh who is currently loaded
		 */
		void LoadVertices(aiMesh* mesh, const unsigned int increaseIndices);

		/**
		 * Load all the indices of one aiMesh
		 * @param mesh: The aiMesh who is currently loaded
		 * @param increaseIndices: The number of with wich we want increase the value of the indices of the mesh
		 * use when multiple mesh are load for one Model
		 */
		void LoadIndices(aiMesh* mesh, const int increaseIndices, const unsigned int indexMesh);
		
		void LoadAnimationVertexDatas(aiMesh* mesh, const unsigned int increaseIndices);

		void LoadAnimationVertexData(aiMesh* mesh, const unsigned int boneIndex, 
										aiBone* currBone, const unsigned int numVertices);


		/**
		 * Link the Model to OpenGL
		 */
		void CreateVAOModel();

		/**
		 * Emplace all the usefull value in the Model
		 */
		void EmplaceInModel();

	};
}