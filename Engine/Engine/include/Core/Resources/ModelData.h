#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
 
#include "Vertex.h" 
#include "OpenGlLinkState.h" 
#include "CoreMinimal.h"


struct aiMesh;

 

namespace Resources
{
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

		std::string ModelName;
		EOpenGLLinkState stateVAO;

		void SetArrays(const aiScene* scene, const unsigned int& index);

		void LoadaiMeshModel(aiMesh* mesh, std::shared_ptr<Loader::ImporterData>& importer, const unsigned int indexMesh = 0, const int increaseIndices = 0);

		void LoadaiMeshAABB(aiMesh* mesh);

		/**
		 * Load all the vertices of one aiMesh
		 * @param mesh: The aiMesh with wich we want to load his vertices
		 */
		void LoadVertices(aiMesh* mesh, const int increaseIndices);

		/**
		 * Load all the indices of one aiMesh
		 * @param mesh: The aiMesh with wich we want to load his indices
		 * @param increaseIndices: The number of with wich we want increase the value of the indices of the mesh
		 * use when multiple mesh are load for one Model
		 */
		void LoadIndices(aiMesh* mesh, const int increaseIndices, const unsigned int indexMesh);

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