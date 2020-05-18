#include <fstream>

#include "ScriptedComponent.h"
#include "Maths.hpp"
#include "Object.hpp"
#include "InputSystem.hpp"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;

	RegisterDefaultClassConstructor<ScriptedComponent>("Script" , ComponentUpdatable(), ComponentBase());
	RegisterClassProperty<ScriptedComponent>("Script", "Lua Script", &ScriptedComponent::m_script);

	lua.create_named_table("Debug",
		"Log", ScriptedComponent::LogWrapper,
		"Warning", ScriptedComponent::WarningWrapper,
		"Error", ScriptedComponent::ErrorWrapper);

	Core::Maths::RegisterMathsForLua(lua);
	Core::SystemManagement::InputSystem::RegisterInputEnums(lua);
}	

namespace Core::Datastructure
{
	ScriptedComponent::ScriptedComponent() : ComponentUpdatable()
	{
		m_env = sol::environment(lua, sol::create, lua.globals());
	}

	ScriptedComponent::ScriptedComponent(const std::string& filename) : ComponentUpdatable()
	{
		m_script = filename;
		LoadLuaScript();
	}

	ScriptedComponent::~ScriptedComponent()
	{
	}

	bool ScriptedComponent::OnStart()
	{	
		bool temp = ComponentUpdatable::OnStart();

		if (m_script.empty())
			return false;

		Core::Datastructure::lua.open_libraries(sol::lib::base);

		if (!m_hasLoaded)
			LoadLuaScript();
		StartLuaScript();

		m_hasStarted = true;

		return temp;
	}

	void ScriptedComponent::LoadLuaScript()
	{
		if (Core::Datastructure::lua.safe_script_file(m_script, m_env))
		{
			std::string loadingMsg = "Script: " + m_script + " didn't load";
			BAKERS_LOG_ERROR(loadingMsg);
			m_script.clear();
		
		}
		else
		{
			m_hasLoaded = true;
			m_start = m_env["Start"];
			m_update = m_env["Update"];

			std::string loadingMsg = "Script: " + m_script + " loaded successfully";
			BAKERS_LOG_MESSAGE(loadingMsg);
		}
	}

	void ScriptedComponent::StartLuaScript()
	{
		m_env["object"] = m_parent;
		m_env["Input"] = Input();

		if (m_start.valid())
			m_start();
		else
		{
			std::string msg = std::string(m_script) + " has no Start function";
			BAKERS_LOG_WARNING(msg);
		}

		// Update is checked in Start method so that warning only appears once
		if (!m_update.valid())
		{
			std::string msg = std::string(m_script) + " has no Update function";
			BAKERS_LOG_WARNING(msg);
		}
	}

	void ScriptedComponent::OnUpdate(float deltaTime)
	{
		if (m_script.empty())
			return;

		// First frame the script is valid
		if (!m_hasStarted)
		{
			m_hasStarted = true;
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
		copy->m_env = m_env;
	}

	void ScriptedComponent::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new ScriptedComponent();
		OnCopy(copyTo);
	}

	void ScriptedComponent::OnReset()
	{
		ComponentUpdatable::OnReset();

		m_script.clear();
	}

	void ScriptedComponent::CreateScript(const std::string& scriptPath)
	{
		std::ofstream file(scriptPath);

		// Create default content (empty Start and Update functions) for new script
		std::string defaultContent = "-- " + scriptPath + " script\n\n";
		defaultContent += "function Start()\n\nend\n\n";
		defaultContent += "function Update()\n\nend";

		// Write content in file
		file << defaultContent;
	}
}