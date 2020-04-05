#pragma once
#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "ICollidable.h"
#include "IPhysics.h"
#include "IUpdatable.hpp"

namespace physx
{
	class PxRigidStatic;
	class PxPhysics;
	class PxScene;
}

namespace Core
{
	namespace Physics
	{

		/**
		 * Contains a collider and a link to a PxRigidStatic from the PhysicsScene
		 */
		BAKERS_API_CLASS StaticMesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IPhysics, public virtual Core::Datastructure::IUpdatable
		{

		protected:
			physx::PxRigidStatic*	m_staticMesh;

			virtual void OnDestroy() override;
			virtual void OnReset() override;

		public:

			/**
			 * Default Constructor
			 */
			StaticMesh() = default;

			/**
			 * Constructor with a Collider, for init the collider directly
			 * @param collider: the collider with wich we want to construct the physics mesh
			 */
			StaticMesh(Collider* collider);

			/**
			*/
			/*inline virtual void SetStaticMesh(physx::PxRigidStatic* staticMesh)
			{
				m_staticMesh = staticMesh;
			}*/

			/*inline virtual physx::PxRigidStatic* GetStaticMesh()
			{
				return m_staticMesh;
			}*/

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * override for call the two OnStart of IPhysics and IUpdatable
			 */
			virtual void OnStart() override;

			/**
			 * Function inheritated from IPhysics,
			 * override for create a specific physX actor a PxRigidStatic
			 * @param physics: the PhysX physics from the PhysicsScene
			 * @param scene: the PhysX scene from the PhysicsScene
			 */
			virtual void CreateActor(physx::PxPhysics* physics, physx::PxScene* scene) override;

			/**
			 * Function inheritated from IPhysics and IUpdatable,
			 * @param deltaTime: Time since previous frame
			 */
			virtual void OnUpdate(float deltaTime) override {};

			REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IPhysics, Core::Datastructure::IUpdatable)
		};
	}
}

