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
	/**
	 * Default Component linked with Lua Script class
	 */
	BAKERS_API_CLASS ScriptedComponent : public ComponentUpdatable
	{
	private:
		const char* m_script = nullptr;
		lua_State* m_lState;

		/**
		 * Copy event for editor component handling
		 * @param copyTo: Store the copy
		 */
		virtual void	OnCopy(void* copyTo) const override;

		/**
		 * Call method for editor component handling
		 * @param copyTo: Store the copy
		 */
		virtual void	StartCopy(void*& copyTo) const override;
	public:
		/**
		 * Default constructor
		 */
		ScriptedComponent();

		/**
		 * Constructor by value
		 * @param fileName: Path to the Lua script file
		 */
		ScriptedComponent(const char* fileName);

		/**
		 * Destructor
		 */
		~ScriptedComponent();

		/**
		 * Set Lua script file
		 * @param fileName: Path to the Lua script file
		 */
		inline void	SetFile(const char* fileName) noexcept { m_script = fileName; };

		/**
		 * Call Start function in Lua script
		 */
		virtual void OnStart() override;

		/**
		 * Call Update function in Lua script
		 * @param deltaTime: Time elapsed between two frames, sent to Lua Update
		 */
		virtual void OnUpdate(float deltaTime) override;

		/**
		 * Get value of given variable
		 * @param name: Name of the desired variable in Lua script
		 * @return: Value of given variable is it exists, default value of given type otherwise
		 */
		template<class T>
		T get(const char* name);

		/**
		 * Template specialization for get method
		 * @param name: Name of the desired variable in Lua script
		 * @return: Value of given variable is it exists, default string otherwise
		 */
		template<>
		std::string get(const char* name);

		REGISTER_CLASS(ComponentUpdatable);
	};

	template<class T>
	T ScriptedComponent::get(const char* name)
	{
		T none = T();

		if (!m_lState)
			return none;

		lua_getglobal(m_lState, name);
		if (lua_isnumber(m_lState, -1))
			return lua_tonumber(m_lState, -1);

		return none;
	}

	template<>
	std::string ScriptedComponent::get(const char* name)
	{
		if (!m_lState)
			return "";

		lua_getglobal(m_lState, name);
		if (lua_isstring(m_lState, -1))
			return lua_tostring(m_lState, -1);

		return "";
	}
}



