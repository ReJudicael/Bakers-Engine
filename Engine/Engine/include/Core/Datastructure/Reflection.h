#pragma once

#include <rttr/registration.h>
#include <rttr/registration_friend.h>
using namespace rttr;


#define REGISTER_CLASS(...) \
RTTR_ENABLE(__VA_ARGS__)			\
RTTR_REGISTRATION_FRIEND		

namespace Core::Datastructure
{

}