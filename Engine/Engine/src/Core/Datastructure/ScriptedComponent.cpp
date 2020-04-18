#include "ScriptedComponent.h"
#include "Vec3.hpp"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;

	RegisterDefaultClassConstructor<ScriptedComponent>("Script" , ComponentUpdatable(), ComponentBase());
	RegisterClassProperty<ScriptedComponent>("Script", "Script", &ScriptedComponent::m_script);
}	

namespace Core::Datastructure
{
	ScriptedComponent::ScriptedComponent() : ComponentUpdatable()
	{
	}

	ScriptedComponent::ScriptedComponent(const std::string& filename) : ComponentUpdatable()
	{
		m_script = filename;
	}

	ScriptedComponent::~ScriptedComponent()
	{
	}

	void ScriptedComponent::OnStart()
	{	
		ComponentUpdatable::OnStart();

		if (m_script.empty())
			return;

		Core::Datastructure::lua.open_libraries(sol::lib::base);

		if (Core::Datastructure::lua.safe_script_file(m_script))
		{
			std::string loadingMsg = m_script + " didn't load";
			BAKERS_LOG_ERROR(loadingMsg);
			m_script.clear();
			return;
		}

		m_start = Core::Datastructure::lua["Start"];
		m_update = Core::Datastructure::lua["Update"];

		if (m_start.valid())
			m_start();
	}

	void ScriptedComponent::OnUpdate(float deltaTime)
	{
		if (m_script.empty())
			return;

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

		m_script.clear();
	}
}