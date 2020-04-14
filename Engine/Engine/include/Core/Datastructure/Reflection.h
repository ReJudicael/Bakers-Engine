#pragma once

#include <sol.hpp>
#include <rttr/registration.h>
#include <rttr/registration_friend.h>
using namespace rttr;


#define REGISTER_CLASS(...) \
RTTR_ENABLE(__VA_ARGS__)			\
RTTR_REGISTRATION_FRIEND		

namespace Core::Datastructure
{
	static sol::state lua;

	template<class T, typename ... parent>
	void RegisterDefaultClassConstructor(const char* className, parent...)
	{
		registration::class_<T>(className)
			.constructor();

		sol::usertype<T> testvar = lua.new_usertype<T>(className,
			sol::constructors<T()>(),
			sol::base_classes, sol::bases<parent...>());
	}

	template<class T, typename ... params>
	void RegisterClassConstructor(const char* className, params...)
	{
		registration::class_<T>(className)
			.constructor<params...>();
	}

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

	template<class T, class M>
	void RegisterClassMethod(const char* className, const char* methodName, M method)
	{
		registration::class_<T>(className)
			.method(methodName, method);

		sol::usertype<T> registeredClass = lua[className];
		registeredClass[methodName] = method;
	}
}