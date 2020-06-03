#pragma once

#include "ComponentUpdatable.h"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	/**
	 * Default Component linked with Lua Script class
	 */
	BAKERS_API_CLASS ScriptedComponent : public ComponentUpdatable
	{
	private:
		std::string     m_script;
		bool            m_hasStarted{ false };
		bool			m_hasLoaded{ false };
		sol::function   m_start;
		sol::function   m_update;

		sol::environment m_env;

		/**
		 * Copy event for editor component handling
		 * @param copyTo: Store the copy
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;

		/**
		 * Call method for editor component handling
		 * @param copyTo: Store the copy
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * Reset event
		 */
		virtual void	OnReset() override;
	public:
		/**
		 * Default constructor
		 */
		ScriptedComponent();

		/**
		 * Constructor by value
		 * @param fileName: Path to the Lua script file
		 */
		ScriptedComponent(const std::string& fileName);

		/**
		 * Destructor
		 */
		~ScriptedComponent();

		/**
		 * Set Lua script file
		 * @param fileName: Path to the Lua script file
		 */
		inline void	SetFile(const std::string& fileName) noexcept { m_script = fileName; };

		/**
		 * Set Lua script and call Start function in script
		 */
		virtual bool OnStart() override;

		/**
		 * Load Start and Update functions of lua script
		 */
		void LoadLuaScript();

		/**
		 * Check and call start function in lua script
		 */
		void StartLuaScript();

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
		T Get(const std::string& name);

		/**
		 * Set access to c++ variable in lua script
		 * @param name: Name of the desired variable in Lua script
		 * @param value: value of the variable
		 */
		template<class T>
		void Set(const std::string& name, const T value);

		/**
		 * Log system message wrapper for lua
		 * @param msg: Message to display in editor console as a log
		 */
		static inline void LogWrapper(std::string msg) { BAKERS_LOG_MESSAGE(msg); }

		/**
		 * Log system error wrapper for lua
		 * @param msg: Message to display in editor console as an error
		 */
		static inline void ErrorWrapper(std::string msg) { BAKERS_LOG_ERROR(msg); }

		/**
		 * Log system warning wrapper for lua
		 * @param msg: Message to display in editor console as a warning
		 */
		static inline void WarningWrapper(std::string msg) { BAKERS_LOG_WARNING(msg); }

		/**
		 * Create a lua file with default script content
		 * @param scriptName: Name of the file to create.
		 * @warning Will not work if a script with the same name already exists.
		 */
		static void CreateScript(const std::string& scriptName);

		REGISTER_CLASS(ComponentUpdatable);
	};

	template<class T>
	T ScriptedComponent::Get(const std::string& name)
	{
		if (m_script.empty())
			return T();

		return Core::Datastructure::lua[name];
	}

	template<class T>
	void ScriptedComponent::Set(const std::string& name, const T value)
	{
		if (m_script.empty())
			return;

		m_env[name] = value;
	}
}



