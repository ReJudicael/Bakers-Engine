#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"

namespace Resources
{
	std::vector<Core::Renderer::Light*>	Shader::lights;

	Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath, EShaderHeaderType header)
	{
		m_shaderHeader = header;

		m_vertexFile = vertexFilePath;
		m_fragmentFile = fragmentFilePath;

		Load();
	}

	Shader::~Shader()
	{
	}

	void	Shader::Delete()
	{
		if (m_programID != UINT_MAX)
			glDeleteProgram(m_programID);
	}

	void Shader::LoadFromFile(const char* file, EShaderType shaderType)
	{
		if (shaderType == EShaderType::VERTEX)
		{
			std::string content = version + GetCodeFromFile(file);
			m_vertex = content.c_str();
		}
		else
		{
			std::string content = version;
			if (m_shaderHeader == EShaderHeaderType::LIGHT)
				content += GetCodeFromFile(lightShaderSource);
			
			content += GetCodeFromFile(file);
			m_fragment = content.c_str();
		}
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
			BAKERS_LOG_WARNING("Can not open: " + std::string(filePath));

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
		std::vector<const char*> Sources;
		Sources.push_back(code);
		glShaderSource(vertexShader, (GLsizei)Sources.size(), &Sources[0], nullptr);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			GLsizei l = 200;
			GLchar* info = new GLchar;
			glGetShaderInfoLog(vertexShader, l, &l, info);
			std::string compileMsg = "Vertex shader " + m_vertexFile + " didn't load: ";
			compileMsg += info;
			BAKERS_LOG_WARNING(compileMsg);
			return;
		}
		
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		code = m_fragment.c_str();
		std::vector<const char*> FragSources;
		FragSources.push_back(code);
		glShaderSource(fragmentShader, (GLsizei)FragSources.size(), &FragSources[0], nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE)
		{
			std::string compileMsg = "Fragment shader " + m_fragmentFile + " didn't load: ";
			GLsizei l = 200;
			GLchar* info = new GLchar;
			glGetShaderInfoLog(fragmentShader, l, &l, info);
			compileMsg += info;
			BAKERS_LOG_WARNING(compileMsg);
			return;
		}

		glAttachShader(m_programID, vertexShader);
		glAttachShader(m_programID, fragmentShader);

		glLinkProgram(m_programID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::string successMsg = "Program made with " + m_vertexFile;
		successMsg += " and " + m_fragmentFile + " loaded successfully";
		BAKERS_LOG_MESSAGE(successMsg);
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

		// Reset vertex and fragment strings that are no longer needed
		m_vertex = "";
		m_fragment = "";
	}

	void	Shader::Load()
	{
		LoadFromFile(m_vertexFile.c_str(), EShaderType::VERTEX);
		LoadFromFile(m_fragmentFile.c_str(), EShaderType::FRAGMENT);

		Compile();
		StoreAllUniforms();
	}

	void	Shader::Reload()
	{
		Delete();
		m_locations.clear();

		Load();
	}

	void Shader::UseProgram()
	{
		glUseProgram(m_programID);
	}

	void Shader::SendLights()
	{
		int activeLightsCount = 0;
		for (int i = 0; i < lights.size(); i++)
		{
			if (lights[i] && lights[i]->IsActive())
			{
				std::string loc = "uLight[" + std::to_string(activeLightsCount) + "].";

				glUniform1i(GetLocation(loc + "type"), int(lights[i]->GetLightType()));
				glUniform3fv(GetLocation(loc + "position"), 1, lights[i]->GetPosition().xyz);
				glUniform3fv(GetLocation(loc + "direction"), 1, lights[i]->GetDirection().xyz);
				glUniform3fv(GetLocation(loc + "ambient"), 1, lights[i]->GetAmbient().rgb);
				glUniform3fv(GetLocation(loc + "diffuse"), 1, lights[i]->GetDiffuse().rgb);
				glUniform3fv(GetLocation(loc + "specular"), 1, lights[i]->GetSpecular().rgb);
				glUniform3fv(GetLocation(loc + "attenuation"), 1, lights[i]->GetAttenuation().xyz);
				glUniform1f(GetLocation(loc + "range"), lights[i]->GetRange());
				glUniform1f(GetLocation(loc + "angle"), lights[i]->GetAngle());
				glUniform1f(GetLocation(loc + "anglesmoothness"), lights[i]->GetAngleSmoothness());
				activeLightsCount++;
			}
		}

		glUniform1i(GetLocation("uLightCount"), activeLightsCount);
	}
}
