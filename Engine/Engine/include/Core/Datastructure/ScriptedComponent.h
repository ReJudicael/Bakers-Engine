#pragma once

#include "ComponentUpdatable.h"
#include "CoreMinimal.h"

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
	BAKERS_API_CLASS ScriptedComponent : public ComponentUpdatable
	{
	private:
		const char* m_script = nullptr;
		lua_State* m_lState;

		virtual void	OnCopy(void* copyTo) const override;
		virtual void	StartCopy(void*& copyTo) const override;
	public:
		ScriptedComponent();
		ScriptedComponent(const char* fileName);
		~ScriptedComponent();

		inline void	SetFile(const char* fileName) noexcept { m_script = fileName; };

		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;

		REGISTER_CLASS(ComponentUpdatable);
	};
}

