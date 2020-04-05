#include "IPhysics.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "BoxCollider.h"
#include "PhysicsScene.h"
#include <functional>


namespace Core
{
	namespace Datastructure
	{
		IPhysics::IPhysics():
			m_collider{new Core::Physics::BoxCollider()}
		{
		}
		IPhysics::IPhysics(Core::Physics::Collider* collider):
			m_collider {collider}
		{
		}

		void IPhysics::OnStart()
		{
			RootObject* root = GetScene();
			root->GetEngine()->GetPhysicsScene()->CreatePhysicsShape(*m_collider);
			root->GetEngine()->GetPhysicsScene()->AttachActor(this);
		}

		void IPhysics::OnDestroy()
		{
			delete m_collider;
		}

		void IPhysics::OnReset()
		{
			IComponent::OnReset();
			delete m_collider;
			m_collider = new Core::Physics::BoxCollider();
		}
	}
}