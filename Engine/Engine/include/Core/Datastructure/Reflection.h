#pragma once

#include <rttr/registration.h>
#include <rttr/registration_friend.h>
using namespace rttr;

#define REGISTER_CLASS(parents) \
RTTR_ENABLE(parents)			\
RTTR_REGISTRATION_FRIEND		

namespace Core::Datastructure
{

}