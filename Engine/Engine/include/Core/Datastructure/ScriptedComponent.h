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
		const char* m_script = nullptr;
		bool		m_hasStarted = false;

		sol::function m_start;
		sol::function m_update;

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
		 * Set start variable to false so that the script gets loaded and started again
		 */
		inline void Restart() noexcept { m_hasStarted = false; };

		/**
		 * Set Lua script and call Start function in script
		 */
		virtual void OnStart() override;

		/**
		 * Load Start and Update functions of lua script
		 * @return True if the script has loaded successfully, false otherwise
		 */
		bool LoadLuaScript();

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
		T Get(const char* name);

		/**
		 * Set access to c++ variable in lua script
		 * @param name: Name of the desired variable in Lua script
		 * @param value: pointer to the variable
		 */
		template<class T>
		void Set(const char* name, const T* value);

		/**
		 * Log system message wrapper for lua
		 * @param msg: Message to display in editor console as log
		 */
		static inline void LogWrapper(std::string msg) { BAKERS_LOG_MESSAGE(msg); }

		/**
		 * Log system error wrapper for lua
		 * @param msg: Message to display in editor console as log
		 */
		static inline void ErrorWrapper(std::string msg) { BAKERS_LOG_ERROR(msg); }
		static inline void WarningWrapper(std::string msg) { BAKERS_LOG_WARNING(msg); }

		REGISTER_CLASS(ComponentUpdatable);
	};

	template<class T>
	T ScriptedComponent::Get(const char* name)
	{
		if (!m_script)
			return T();

		return Core::Datastructure::lua[name];
	}

	template<class T>
	void ScriptedComponent::Set(const char* name, const T* value)
	{
		if (!m_script)
			return;

		Core::Datastructure::lua[name] = value;
	}
}



