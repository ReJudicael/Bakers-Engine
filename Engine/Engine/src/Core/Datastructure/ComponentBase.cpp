#include "ComponentBase.h"

RTTR_REGISTRATION
{
	registration::class_<Core::Datastructure::ComponentBase>("ComponentBase")
		.constructor<>()
		.constructor<const Core::Datastructure::ComponentBase&>();
}