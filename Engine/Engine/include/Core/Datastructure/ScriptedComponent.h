#pragma once

#include "ComponentUpdatable.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#ifdef _WIN64
#pragma comment(lib, "lua53.lib")
#endif

namespace Core::Datastructure
{
	class ScriptedComponent : public ComponentUpdatable
	{
		const char* m_script = nullptr;
		lua_State* m_lState;

	public:
		ScriptedComponent();
		ScriptedComponent(const char* fileName);
		~ScriptedComponent();

		inline void	SetFile(const char* fileName) noexcept { m_script = fileName; };

		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
	};
}

