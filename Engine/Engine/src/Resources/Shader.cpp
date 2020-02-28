#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"

namespace Resources
{
	Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
	{
		LoadFromFile(vertexFilePath, EShaderType::VERTEX);
		LoadFromFile(fragmentFilePath, EShaderType::FRAGMENT);
	}

	Shader::~Shader()
	{
		if (m_programID != UINT_MAX)
			glDeleteProgram(m_programID);
	}

	void Shader::LoadFromFile(const char* file, EShaderType shaderType)
	{
		if (shaderType == EShaderType::VERTEX)
			m_vertex = GetCodeFromFile(file).c_str();
		else
			m_fragment = GetCodeFromFile(file).c_str();
	}

	std::string	Shader::GetCodeFromFile(const char* filePath)
	{
		std::string code;
		std::ifstream stream(filePath, std::ios::in);
		if (stream.is_open())
		{
			std::stringstream sstr;
			sstr << stream.rdbuf();
			code = sstr.str();
			stream.close();
		}
		else
			std::cout << "Can not open " << filePath << std::endl;

		return code;
	}

	GLint Shader::GetLocation(std::string uniformName)
	{
		if (m_locations.find(uniformName) != m_locations.end())
			return m_locations[uniformName];

		m_locations[uniformName] = glGetUniformLocation(m_programID, uniformName.c_str());
		return m_locations[uniformName];
	}

	void Shader::Compile()
	{
		m_programID = glCreateProgram();
		GLint compileStatus;

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* code = m_vertex.c_str();
		glShaderSource(vertexShader, (GLsizei)code, &code, nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			std::cout << "Vertex shader didn't load" << std::endl;
			return;
		}
		
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		code = m_fragment.c_str();
		glShaderSource(fragmentShader, (GLsizei)code, &code, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			std::cout << "Fragment shader didn't load" << std::endl;
			return;
		}
		std::cout << "compiling" << std::endl;
		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);

		glLinkProgram(m_programID);


		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::cout << "program load" << std::endl;
	}

	void	Shader::StoreUniformsFromCode(std::string file)
	{
		size_t pos = 0, secondPos = 0;

		while (pos != std::string::npos)
		{
			pos = file.find("uniform", pos);
			if (pos == std::string::npos)
				return;

			pos = file.find_first_not_of(" ", pos + 7);
			pos = file.find_first_of(" ", pos + 1);
			pos = file.find_first_not_of(" ", pos);
			secondPos = file.find_first_of(" ;", pos);
			std::string newUniform = file.substr(pos, secondPos - pos);
			GetLocation(newUniform);
		}
	}

	void	Shader::StoreAllUniforms()
	{
		StoreUniformsFromCode(m_vertex);
		StoreUniformsFromCode(m_fragment);
	}
}
