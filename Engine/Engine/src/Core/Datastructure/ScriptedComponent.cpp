#include "ScriptedComponent.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Datastructure::ScriptedComponent>("ScriptedComponent")
		.constructor()
		.constructor<const char*>()
		.property("Script", &Core::Datastructure::ScriptedComponent::m_script);
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

		m_lua.open_libraries(sol::lib::base);
		if (!m_lua.script_file(m_script).valid())
		{
			std::cout << "Failed to load " << m_script << std::endl;
			m_script = nullptr;
			return;
		}

		m_lua["Start"]();
	}

	void ScriptedComponent::OnUpdate(float deltaTime)
	{
		if (!m_script)
			return;

		m_lua["Update"](deltaTime);
	}

	void ScriptedComponent::OnCopy(IComponent* copyTo) const
	{
		ComponentUpdatable::OnCopy(copyTo);
		ScriptedComponent* copy{ dynamic_cast<ScriptedComponent*>(copyTo) };

		copy->m_script = m_script;
		copy->m_lua.script_file(copy->m_script);
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
}