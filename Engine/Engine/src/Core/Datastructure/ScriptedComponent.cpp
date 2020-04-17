#include <fstream>

#include "ScriptedComponent.h"
#include "Vec3.hpp"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;

	RegisterDefaultClassConstructor<ScriptedComponent>("ScriptedComponent" , ComponentUpdatable(), ComponentBase());
	RegisterClassProperty<ScriptedComponent>("ScriptedComponent", "Script", &ScriptedComponent::m_script);

	lua.create_named_table("Debug",
		"Log", ScriptedComponent::LogWrapper,
		"Error", ScriptedComponent::ErrorWrapper,
		"Warning", ScriptedComponent::WarningWrapper);
}	

namespace Core::Datastructure
{
	ScriptedComponent::ScriptedComponent() : ComponentUpdatable()
	{
	}

	ScriptedComponent::ScriptedComponent(const char* filename) : ComponentUpdatable()
	{
		m_script = filename;
	}

	ScriptedComponent::~ScriptedComponent()
	{
	}

	void ScriptedComponent::OnStart()
	{	
		ComponentUpdatable::OnStart();

		if (!m_script)
			return;

		Core::Datastructure::lua.open_libraries(sol::lib::base);

		LoadLuaScript();
		StartLuaScript();
	}

	bool ScriptedComponent::LoadLuaScript()
	{
		if (Core::Datastructure::lua.safe_script_file(m_script))
		{
			std::string loadingMsg = std::string(m_script) + " didn't load";
			BAKERS_LOG_ERROR(loadingMsg);
			m_script = nullptr;
			return false;
		}

		m_start = Core::Datastructure::lua["Start"];
		m_update = Core::Datastructure::lua["Update"];

		return true;
	}

	void ScriptedComponent::StartLuaScript()
	{
		if (m_start.valid())
			m_start();
		else
		{
			std::string msg = std::string(m_script) + " has no Start function";
			BAKERS_LOG_WARNING(msg);
		}

		// Start only need to be checked and called at first frame
		// So will not check again even if m_start was not valid
		m_hasStarted = true;

		// Update is checked in Start method so that warning only appears once
		if (!m_update.valid())
		{
			std::string msg = std::string(m_script) + " has no Update function";
			BAKERS_LOG_WARNING(msg);
		}
	}

	void ScriptedComponent::OnUpdate(float deltaTime)
	{
		if (!m_script)
			return;

		// First frame the script is valid
		if (!m_hasStarted)
		{
			if (LoadLuaScript())
				StartLuaScript();
			return; // First lua Update will be called next frame
		}

		if (m_update.valid())
			m_update(deltaTime);
	}

	void ScriptedComponent::OnCopy(IComponent* copyTo) const
	{
		ComponentUpdatable::OnCopy(copyTo);
		ScriptedComponent* copy{ dynamic_cast<ScriptedComponent*>(copyTo) };

		copy->m_script = m_script;
	}

	void ScriptedComponent::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new ScriptedComponent();
		OnCopy(copyTo);
	}

	void ScriptedComponent::OnReset()
	{
		ComponentUpdatable::OnReset();

		m_script = nullptr;
	}

	void ScriptedComponent::CreateScript(std::string scriptName)
	{
		std::string path = "Resources/Scripts/" + scriptName + ".lua";

		// Check if script already exists
		if (FILE* file = fopen(path.c_str(), "r"))
		{
			fclose(file);
			BAKERS_LOG_ERROR(scriptName + " already exists");
			return;
		}
		
		// Create file
		std::ofstream file(path);

		// Create default content (empty Start and Update functions) for new script
		std::string defaultContent = "-- " + scriptName + " script\n\n";
		defaultContent += "function Start()\n\nend\n\n";
		defaultContent += "function Update()\n\nend";

		// Write content in file
		file << defaultContent;
	}
}