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
		std::string m_script;
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
		 
		ScriptedComponent(int i) {};

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
		T get(const std::string& name);

		/**
		 * Set value of given variable
		 * @param name: Name of the desired variable in Lua script
		 * @param value: New value for given variable
		 */
		template<class T>
		void set(const std::string& name, const T& value);


		REGISTER_CLASS(ComponentUpdatable);
	};

	template<class T>
	T ScriptedComponent::get(const std::string& name)
	{
		if (m_script.empty())
			return T();

		return Core::Datastructure::lua[name];
	}

	template<class T>
	void ScriptedComponent::set(const std::string& name, const T& value)
	{
		if (m_script.empty())
			return;

		Core::Datastructure::lua[name] = value;
	}
}



