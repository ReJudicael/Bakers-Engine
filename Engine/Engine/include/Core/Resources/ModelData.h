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

	struct ModelData
	{
		GLuint VAOModel{0};
		bool				haveTangent;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<OffsetMesh> offsetsMesh;
		std::shared_ptr<Model> model;

		std::string ModelName;
		EOpenGLLinkState stateVAO;

		void LoadVertices(aiMesh* mesh);
		void LoadIndices(aiMesh* mesh, const int increaseIndices = 0);

		void CreateVAOModel();

		void EmplaceInModel();

	};
}