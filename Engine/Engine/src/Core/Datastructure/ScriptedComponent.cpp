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
		if (m_script && m_lState)
			lua_close(m_lState);
	}

	void ScriptedComponent::OnStart()
	{
		ComponentUpdatable::OnStart();

		if (!m_script)
			return;

		m_lState = luaL_newstate();
		luaL_openlibs(m_lState);
		if (luaL_dofile(m_lState, m_script) != LUA_OK)
		{
			std::cout << "Lua file " << m_script << "Didn't load" << std::endl;
			lua_close(m_lState);
			m_lState = nullptr;
			m_script = nullptr;
			return;
		}

		lua_getglobal(m_lState, "Start");
		if (lua_isfunction(m_lState, -1))
			lua_pcall(m_lState, 0, 0, 0);
	}

	void ScriptedComponent::OnUpdate(float deltaTime)
	{
		if (!m_lState)
			return;

		lua_getglobal(m_lState, "Update");
		if (lua_isfunction(m_lState, -1))
		{
			lua_pushnumber(m_lState, deltaTime);

			lua_pcall(m_lState, 1, 0, 0);
		}
	}
}