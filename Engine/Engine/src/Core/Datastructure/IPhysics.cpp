#include "IPhysics.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "PhysicsScene.h"


namespace Core
{
	namespace Datastructure
	{
		IPhysics::IPhysics(Core::Physics::Collider collider):
			m_collider {collider}
		{}

		void IPhysics::OnStart()
		{
			RootObject* root = GetScene();
			root->GetEngine()->GetPhysicsScene()->CreateBoxShape(m_collider);
			root->GetEngine()->GetPhysicsScene()->AttachActor(this);
		}
	}
}