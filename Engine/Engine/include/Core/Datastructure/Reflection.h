#pragma once

#include <sol.hpp>
#include <rttr/registration.h>
#include <rttr/registration_friend.h>
using namespace rttr;

enum class MetaData_Type
{
	SHOW_IN_EDITOR, //Put either a bool or a string in this metadata. 
					//The bool will tell the editor to enable or disable display of this property constantly.
					//The string will call the method by the same name in the object.
	STRING_EDITABLE,//Put either a bool or a string in this metadata. 
					//The bool will tell the editor that you can edit the text of the string directly
					//The string will call the method by the same name in the object.
};


#define REGISTER_CLASS(...) \
RTTR_ENABLE(__VA_ARGS__)			\
RTTR_REGISTRATION_FRIEND			

namespace Core::Datastructure
{
	extern sol::state lua;

	/**
	 * Create class in both rttr and lua
	 * @param className: Name that will be used for the class
	 * @param parent: Every parent of the class
	 */
	template<class T, typename ... parent>
	void RegisterDefaultClassConstructor(const char* className, parent...)
	{
		registration::class_<T>(className)
			.constructor();

		lua.new_usertype<T>(className,
			sol::constructors<T()>(),
			sol::base_classes, sol::bases<parent...>());
	}

	/**
	 * Register class constructor with values, only works with rttr
	 * @param className: Name that will be used for the class
	 * @param params: Each type needed for the constructor
	 */
	template<class T, typename ... params>
	void RegisterClassConstructor(const char* className, params...)
	{
		registration::class_<T>(className)
			.constructor<params...>();
	}

	/**
	 * Register property for given class
	 * @param className: Name of the class from which property is registered
	 * @param propertyName: Name used for the property
	 * @param property: Reference to the property
	 * @param access: rttr access level
	 * @param readonly: rttr access
	 */
	template<class T, class P, typename acc_level = detail::public_access>
	void RegisterClassProperty(const char* className, const char* propertyName, P property, acc_level access = detail::public_access(), bool readonly = false)
	{
		sol::usertype<T> registeredClass = lua[className];
		
		if (readonly)
		{
			registeredClass[propertyName] = sol::readonly(property);

			registration::class_<T>(className)
				.property_readonly(propertyName, property, access);
		}
		else
		{
			registeredClass[propertyName] = property;

			registration::class_<T>(className)
				.property(propertyName, property, access);
		}	
	}

	/**
	 * Register method for given class
	 * @param className: Name of the class from which method is registered
	 * @param methodName: Name used for the method
	 * @param method: Reference to the method
	 */
	template<class T, class M>
	void RegisterClassMethod(const char* className, const char* methodName, M method)
	{
		registration::class_<T>(className)
			.method(methodName, method);

		sol::usertype<T> registeredClass = lua[className];
		registeredClass[methodName] = method;
	}

	/**
	 * Register property for given class with getter and setter methods
	 * @param className: Name of the class from which property are registered
	 * @param propertyName: Name used for the property
	 * @param getter: Reference to the property getter
	 * @param setter: Reference to the property setter
	 * @param access: rttr access level
	 */
	template<class T, class G, class S, typename acc_level = detail::public_access>
	void RegisterClassPropertyGS(const char* className, const char* propertyName, G getter, S setter, acc_level access = detail::public_access())
	{
		registration::class_<T>(className)
			.property(propertyName, getter, setter, access);

		std::string getName = "get" + std::string(propertyName);
		std::string setName = "set" + std::string(propertyName);
		RegisterClassMethod<T>(className, getName.c_str(), getter);
		RegisterClassMethod<T>(className, setName.c_str(), setter);
	}

	/**
	 * Register lua enumeration inside class
	 * @param className: Name of the class in which the enum is
	 * @param enumName: Name used for the enum
	 * @param args: Values of the enum
	 */
	template<class T, class ...Args>
	void RegisterLuaEnumeration(const char* className, const char* enumName, Args&&... args)
	{
		sol::usertype<T> registeredClass = lua[className];
		registeredClass[enumName] = lua.create_table_with(args...);
	}

	/**
	 * Register global lua enumeration
	 * @param enumName: Name used for the enum
	 * @param args: Values of the enum
	 */
	template<class ...Args>
	void RegisterLuaEnumeration(const char* enumName, Args&&... args)
	{
		lua.new_enum(enumName, args...);
	}
}