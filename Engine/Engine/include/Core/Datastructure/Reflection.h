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

	template<class T>
	void RegisterDefaultClassConstructor(const char* className)
	{
		registration::class_<T>(className)
			.constructor();

		sol::usertype<T> testvar = lua.new_usertype<T>(className,
			sol::constructors<T()>());
	}

	template<class T, class P>
	void RegisterClassProperty(const char* className, const char* propertyName, P property)
	{
		registration::class_<T>(className)
			.property(propertyName, property);

		sol::usertype<T> registeredClass = lua[className];
		registeredClass[propertyName] = property;
	}
}