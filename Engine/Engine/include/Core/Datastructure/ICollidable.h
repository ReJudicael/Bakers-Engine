#pragma once
#include "CoreMinimal.h"
#include "IComponent.hpp"

namespace physx
{
	class PxPhysics;
}

namespace Core::Datastructure
{
	class ICollidable : public virtual IComponent
	{
	public:
		virtual void OnDestroy() override {}
		virtual void OnStart() override;

		virtual void UpdatePhysics() {};

		REGISTER_CLASS(IComponent)
	};
}

