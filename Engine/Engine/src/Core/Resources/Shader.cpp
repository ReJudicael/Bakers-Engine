#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "json.hpp"
using nlohmann::json;

#include "Shader.h"

namespace Resources
{
	RTTR_PLUGIN_REGISTRATION
	{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
		registration::class_<Resources::Shader>("Shader")
		.constructor()
		.property("Vertex", &Resources::Shader::GetVertName, &Resources::Shader::SetVertName)
		.property("Fragment", &Resources::Shader::GetFragName, &Resources::Shader::SetFragName)
		//.property("Header", &Resources::Material::variants)
		;
	}

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

	void Shader::SetFragName(std::string name)
	{
		if (name.find(".frag") == std::string::npos)
			return;
		
		m_fragmentFile = name;
		Load();
	}

	void Shader::SetVertName(std::string name)
	{
		if (name.find(".vert") == std::string::npos)
			return;

		m_vertexFile = name;
		Load();
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
			std::string content = version;
			if (m_shaderHeader == EShaderHeaderType::LIGHT)
				content += GetCodeFromFile(lightShaderSource);

			content += GetCodeFromFile(file);
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
		ZoneScoped
		if (m_locations.find(uniformName) != m_locations.end())
			return m_locations[uniformName];

		m_locations[uniformName] = glGetUniformLocation(m_programID, uniformName.c_str());
		return m_locations[uniformName];
	}

	void Shader::SaveShader(const std::string& path)
	{
		std::ofstream save(path.c_str());
		if (!save.is_open())
			return;

		json jsave;
		jsave["Vertex File"] = m_vertexFile;
		jsave["Fragment File"] = m_fragmentFile;

		jsave["Shader Header"] = static_cast<int>(m_shaderHeader);

		save << std::setw(4) << jsave;

	}

	bool Shader::LoadShader(const std::string& path)
	{
		std::ifstream load(path.c_str());
		if(!load.is_open())
			return false;
		json jload;

		load >> jload;

		m_vertexFile = jload["Vertex File"];
		m_fragmentFile = jload["Fragment File"];
		m_shaderHeader = static_cast<Shader::EShaderHeaderType>(static_cast<int>(jload["Shader Header"]));

		Load();

		return true;

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

		std::string successMsg = "Shaders: " + m_vertexFile;
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
		ZoneScoped
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

	std::vector<Core::Renderer::Light*> Shader::GetShadowCastingLights()
	{
		std::vector<Core::Renderer::Light*> selectedLights;

		for (size_t i{ 0 }; i < Shader::lights.size(); ++i)
		{
			if (Shader::lights[i]->CanCastShadow())
				selectedLights.push_back(Shader::lights[i]);
		}

		return selectedLights;
	}
}
