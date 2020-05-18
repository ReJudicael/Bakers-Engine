#include <functional>
#include "IPhysics.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "BoxCollider.h"
#include "PhysicsScene.h"


namespace Core
{
	namespace Datastructure
	{
		RTTR_PLUGIN_REGISTRATION
		{
			registration::class_<IPhysics>("IPhysics")
			.enumeration<Core::Physics::EFilterRaycast>("Layout")
			(
			value("GROUPE1", Core::Physics::EFilterRaycast::GROUPE1),
			value("GROUPE2", Core::Physics::EFilterRaycast::GROUPE2),
			value("GROUPE3", Core::Physics::EFilterRaycast::GROUPE3),
			value("GROUPE4", Core::Physics::EFilterRaycast::GROUPE4)
			)
			.property("LayerMask", &Core::Datastructure::IPhysics::GetRaycastFilter,
						&Core::Datastructure::IPhysics::SetRaycastFilter)
			;
		}

		IPhysics::IPhysics():
			m_collider{ new Core::Physics::BoxCollider() }
		{
		}
		IPhysics::IPhysics(Core::Physics::Collider* collider):
			m_collider {collider}
		{
		}

		void IPhysics::OnInit()
		{
			RootObject* root = GetRoot();
			//m_collider = new Core::Physics::BoxCollider(GetScene()->GetEngine()->GetResourcesManager());
			root->GetEngine()->GetPhysicsScene()->CreatePhysicsShape(*m_collider);
			root->GetEngine()->GetPhysicsScene()->AttachActor(this);
			m_collider->InitShader(root->GetEngine()->GetResourcesManager()->GetShader("Wireframe"));
			m_collider->InitModel(root->GetEngine()->GetResourcesManager()->GetModel("Cube"));
			IComponent::OnInit();
			IRenderable::OnInit();
		}
		
		void IPhysics::OnCopy(IComponent* copyTo) const
		{
			IComponent::OnCopy(copyTo);

			IPhysics* phy = dynamic_cast<IPhysics*>(copyTo);
			
			phy->m_collider = m_collider;
			phy->m_MeshChangeGlobalPos = m_MeshChangeGlobalPos;
			phy->OnTriggerEnterEvent = OnTriggerEnterEvent;
			phy->OnTriggerExitEvent = OnTriggerExitEvent;
			phy->OnContactEvent = OnContactEvent;
		}

		void IPhysics::OnDraw(Core::Datastructure::ICamera* cam)
		{
			m_collider->DrawCollider(cam, GetParent()->GetGlobalPos(), GetParent()->GetGlobalRot());
		}

		void IPhysics::OnDestroy()
		{
			delete m_collider;
		}

		void IPhysics::OnReset()
		{
			IComponent::OnReset();
			delete m_collider;
		}
	}
}