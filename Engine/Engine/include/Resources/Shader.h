#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>

namespace Resources
{
	class Shader
	{
	public:
		enum class EShaderType : unsigned int
		{
			VERTEX = 0,
			FRAGMENT
		};

	private:
		GLuint m_programID = UINT_MAX;
		std::unordered_map<std::string, GLint> m_locations;

		std::string m_vertex{ "" };
		std::string m_fragment{ "" };

	public:
		Shader(const char* vertexFilePath, const char* fragmentFilePath);
		~Shader();

		void LoadFromFile(const char* file, EShaderType shaderType);

		std::string	GetCodeFromFile(const char* filePath);

		GLint	GetLocation(std::string uniformName);

		void	StoreUniformsFromCode(std::string file);

		void	StoreAllUniforms();

		void Compile();
	};
}
