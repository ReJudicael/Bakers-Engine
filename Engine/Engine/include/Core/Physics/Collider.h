#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vec3.hpp"
#include "Quaternion.hpp"
#include "CoreMinimal.h"
#include "IComponent.hpp"
#include "IRenderable.hpp"
#include "Model.h"
#include "HitResult.h"
#include "EventSystem.hpp"


namespace physx
{
	class PxShape;
	class PxPhysics;
	class PxMaterial;
	class PxRigidActor;
	class PxRigidDynamic;
}
namespace Resources
{
	class Shader;
	namespace Loader
	{
		class ResourcesManager;
	}
}
namespace Core
{
	namespace Maths
	{
		struct Mat4;
	}
	namespace Datastructure
	{
		class ICamera;
		class Transform;
	}
	namespace Physics
	{
		enum class EFilterRaycast : int;

		class RigidBody;

		struct ColliderSave
		{
			Core::Maths::Vec3	localPosition;
			Core::Maths::Quat	localRotation;
			Core::Maths::Vec3	extent;
			Core::Maths::Vec3	physicsMaterial;
			bool				isTrigger;
			EFilterRaycast		raycastFilter;
		};

		/**
		 * Contains the shape and the material of the physics mesh 
		 * with wich he is attached
		 */
		BAKERS_API_CLASS Collider : public virtual Core::Datastructure::IComponent, public virtual Core::Datastructure::IRenderable
		{

		protected:
			ColliderSave*						m_tmpColliderSave{nullptr};
			physx::PxShape*						m_pxShape{};
			physx::PxMaterial*					m_pxMaterial{};
			physx::PxRigidActor*				m_pxRigidActor{};
			std::shared_ptr<Resources::Shader>	m_shader{};
			std::shared_ptr<Resources::Model>	m_model{};
			int									m_IDFunctionSetTRS{0};

		public:
			Core::SystemManagement::EventSystem<Collider*>	OnTriggerEnterEvent;
			Core::SystemManagement::EventSystem<Collider*>	OnTriggerExitEvent;
			Core::SystemManagement::EventSystem<Collider*, Core::Maths::Vec3, Core::Physics::HitResult>	OnContactEvent;
		
		protected:

			virtual void OnCopy(IComponent* copyTo) const override;

			virtual ColliderSave* SaveCollider() const;

			/**
			 * Function inheritated from IComponent,
			 * override for delete the collider
			 */
			virtual void OnDestroy() override;


			virtual void OnReset() override;

			virtual void SetToDefault();


			virtual bool OnStart() override;
		public:
			Collider() = default;

			/**
			 * Function inheritated from IComponent,
			 * override for create the collider from the PhysicsScene
			 */
			virtual void OnInit() override;

			virtual void OnDraw(Core::Datastructure::ICamera* cam) override = 0;

			void DestroyRigidActor();
			
			void DetachShape();

			void CreateActor();

			void InitRigidBody(Core::Physics::RigidBody* rigidBody, int& ID, physx::PxRigidDynamic*& pxRigidBody);


			/**
			 * Get the PhysX shape of the collider
			 * @return the PhysX shape of the collider 
			 */
			physx::PxShape* GetShape()
			{
				return m_pxShape;
			}

			/**
			 * Set the local position of the shape compared to 
			 * the physics mesh with wich he is attached
			 * @param pos: the local position we want to give to the shape
			 */
			virtual void SetLocalPosition(Core::Maths::Vec3 pos);

			/**
			 * Set the local position of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local position of the shape
			 */
			virtual Core::Maths::Vec3 GetLocalPosition() const;

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param quat: the local rotation we want to give to the shape with quaternion
			 */
			virtual void SetLocalRotationQuat(const Core::Maths::Quat& quat);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local rotation of the shape in quaternion
			 */
			virtual Core::Maths::Quat GetLocalRotationQuat() const;

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param euler: the local rotation we want to give to the shape with Euler angle
			 */
			virtual void SetLocalRotationEuler(Core::Maths::Vec3 euler);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the local rotation in Euler angle
			 */
			virtual Core::Maths::Vec3 GetLocalRotationEuler() const;

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @param vec: the material of the shape, StaticFriction, DynamicFriction, Restitution
			 */
			virtual void SetMaterial(Core::Maths::Vec3 mat);

			/**
			 * Set the local rotation of the shape compared to
			 * the physics mesh with wich he is attached
			 * @return the material of the shape, StaticFriction, DynamicFriction, Restitution
			 */
			virtual Core::Maths::Vec3 GetMaterial() const;

			/**
			 * create the shape of the collider from the PhysicsScene
			 * @param physics: the PhysX physics from the PhysicsScene
			 */
			virtual void CreateShape(physx::PxPhysics* physics) = 0;

			/**
			 * Set the trigger state
			 * @param trigger: booleen who set to true or false the state trigger
			 */
			virtual void Trigger(bool trigger);

			/**
			 * return the trigger state of the shape
			 * @return true if the shape is a trigger
			 */
			virtual bool IsTrigger() const;

			/**
			 * Set the state trigger of the shape
			 */
			virtual void TriggerCollider();

			/**
			 * Set the state simulation of the shape
			 */
			virtual void SimulationCollider();

			virtual void InitShader(std::shared_ptr<Resources::Shader> shader)
			{
				m_shader = shader;
			}

			virtual void InitModel(std::shared_ptr<Resources::Model> model)
			{
				m_model = model;
			}

			virtual void SetRaycastFilter(const EFilterRaycast& filter);

			virtual EFilterRaycast GetRaycastFilter() const;

			virtual void DrawCollider(Core::Datastructure::ICamera* cam, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot) = 0;

			/*
			 * Destroy the shape and his pxMaterial
			 */
			virtual void DestroyShape();

			virtual void SetPhysicsTransformParent();


			virtual ~Collider();
			REGISTER_CLASS(Core::Datastructure::IComponent, Core::Datastructure::IRenderable)
		};
	}
}
