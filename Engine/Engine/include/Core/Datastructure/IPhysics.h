#pragma once
#include "IComponent.hpp"
#include "Collider.h"
#include "CoreMinimal.h"
#include "EventSystem.hpp"
#include "IRenderable.hpp"
#include "HitResult.h"

namespace physx
{
	class PxPhysics;
	class PxScene;
}
namespace Core
{
	
	namespace Datastructure
	{
		/**
		 * Components interface for the physics propeties 
		 * give acces to the collider of the physics and
		 * the event OnTriggerEnter/End and OnContact.
		 */
		BAKERS_API_CLASS IPhysics : public virtual IComponent, public virtual IRenderable
		{
		protected:
			Physics::Collider*		m_collider;
			bool					m_MeshChangeGlobalPos{ false };

			virtual void			OnReset() override;
		public:
			Core::SystemManagement::EventSystem<IPhysics*>	OnTriggerEnterEvent;
			Core::SystemManagement::EventSystem<IPhysics*>	OnTriggerExitEvent;
			Core::SystemManagement::EventSystem<IPhysics*, Core::Maths::Vec3, Core::Physics::HitResult>	OnContactEvent;


			/**
			 * get the collider of the Interface
			 * @return The collider of the physics mesh
			 */
			inline virtual Physics::Collider* GetCollider()
			{
				return m_collider;
			}

			/**
			 * Default Constructor, init the collider as a BoxCollider
			 */
			IPhysics();
			
			/**
			 * Constructor with a Collider, for init the collider directly
			 * @param collider: the collider with wich we want to construct the physics mesh
			 */
			IPhysics(Physics::Collider* collider);

			/**
			 * Function inheritated from IComponent,
			 * override for create the collider from the PhysicsScene
			 */
			virtual void OnInit() override;

			virtual void OnCopy(IComponent* copyTo) const;

			/**
			 * Function inheritated from IComponent,
			 * override for delete the collider
			 */
			virtual void OnDestroy() override;

			virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr) override;

			/**
			 * Create a physics mesh from the PhysicsScene, call in the OnStart
			 * @param physics: the PhysX physics from the PhysicsScene
			 * @param scene: the PhysX scene from the PhysicsScene
			 */
			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) = 0;


			/*
			 * Function call as an event whne the Transform of the Object change	 
			 */
			virtual void SetPhysicsTransformParent() = 0;


			virtual void SetRaycastFilter(Core::Physics::EFilterRaycast filter)
			{
				m_collider->SetRaycastFilter(filter);
			}


			virtual Core::Physics::EFilterRaycast GetRaycastFilter()
			{
				return m_collider->GetRaycastFilter();
			}

			REGISTER_CLASS(IComponent, IRenderable)
		};
	}
}

