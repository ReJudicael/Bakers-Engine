#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <string>

namespace Resources
{
	/**
	 * Handle program creation through vertex and fragment shader files
	 */
	class Shader
	{
	public:
		/**
		 * Indicate the type of shader
		 */
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
		/**
		 * Value constructor
		 * @param vertexFilePath: Path to the vertex shader file
		 * @param fragmentFilePath: Path to the fragment shader file
		 */
		Shader(const char* vertexFilePath, const char* fragmentFilePath);

		/**
		 * Destructor
		 */
		~Shader();

		/**
		 * Delete glProgram stored in the shader
		 */
		void	Delete();

		/**
		 * Load shader from file and store its content according to the given type
		 * @param file: Path to the file from which the shader will be load
		 * @param shaderType: Whether the given file is a vertex shader or fragment shader
		 */
		void LoadFromFile(const char* file, EShaderType shaderType);

		/**
		 * Read content from a text file and store it in a string
		 * @param filePath: Path to the file from which the content will be read
		 * @return String with content of given file
		 */
		std::string	GetCodeFromFile(const char* filePath);

		/**
		 * Get glUniformLocation of asked variable
		 * @return Location of asked variable
		 */
		GLint	GetLocation(std::string uniformName);

		/**
		 * Compile vertex and fragment shader to create glProgram
		 */
		void Compile();

		/**
		 * Parse given string to find all uniform variables name and store their glLocation
		 * @param file: String with the content of the shader to parse
		 */
		void	StoreUniformsFromCode(std::string file);

		/**
		 * Store uniforms of both vertex shader and fragment shader
		 */
		void	StoreAllUniforms();

		/**
		 * Use glProgram
		 */
		void UseProgram();
	};
}
