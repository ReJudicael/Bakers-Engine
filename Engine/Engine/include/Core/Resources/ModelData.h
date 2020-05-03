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


	/** 
	 * Class use for create a Model, 
	 * contains all the values use for link a Model to OpenGL
	 * and for animated Mesh
	 */
	struct ModelData
	{
		GLuint VAOModel{0};
		bool											haveTangent;
		bool											haveBones;
		std::vector<Vertex>								vertices;
		std::vector<GLuint>								indices;
		std::vector<OffsetMesh>							offsetsMesh;
		Core::Maths::Vec3								min;
		Core::Maths::Vec3								max;
		std::shared_ptr<Model>							model;
		std::vector<Animation::AnimationVertexData>		modelAnimationData;

		std::string ModelName;
		EOpenGLLinkState stateVAO;

		void LoadaiMeshModel(aiMesh* mesh, const int increaseIndices = 0);

		void SetArray(const aiScene* scene, const bool& isSkeletal);

		void LoadaiMeshAABB(aiMesh* mesh);

		/**
		 * Load all the vertices of one aiMesh
		 * @param mesh: The aiMesh with wich we want to load his vertices
		 */
		void LoadVertices(aiMesh* mesh);

		/**
		 * Load all the indices of one aiMesh
		 * @param mesh: The aiMesh with wich we want to load his indices
		 * @param increaseIndices: The number of with wich we want increase the value of the indices of the mesh
		 * use when multiple mesh are load for one Model
		 */
		void LoadIndices(aiMesh* mesh, const int increaseIndices = 0);

		void LoadAnimationVertexData(aiMesh* mesh, const unsigned int& boneIndex, 
										aiBone* currBone, const unsigned int& numVertices);

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