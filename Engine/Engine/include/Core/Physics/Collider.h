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
			bool				isTrigger{ false };
			bool				isActive{ true };
			EFilterRaycast		raycastFilter;
		};

		/**
		 * Contains the shape and the material of the physics mesh 
		 * with wich he is attached
		 */
		BAKERS_API_CLASS Collider : public virtual Core::Datastructure::IComponent
		{

		protected:
			ColliderSave*						m_tmpColliderSave{nullptr};
			physx::PxShape*						m_pxShape{};
			physx::PxMaterial*					m_pxMaterial{};
			physx::PxRigidActor*				m_pxRigidActor{};
			int									m_IDFunctionSetTRS{0};

		public:
			Core::SystemManagement::EventSystem<Collider*>	OnTriggerEnterEvent;
			Core::SystemManagement::EventSystem<Collider*>	OnTriggerExitEvent;
			Core::SystemManagement::EventSystem<Collider*, Core::Maths::Vec3, Core::Physics::HitResult>	OnContactEvent;
		
		protected:
			/**
			 * Function inheritated from IComponent,
			 * override for copy as a collider
			 */
			virtual void OnCopy(IComponent* copyTo) const override;

			/**
			 * Create a save of the collider 
			 */
			virtual ColliderSave* SaveCollider() const;

			/**
			 * Function inheritated from IComponent,
			 * override for delete the collider
			 */
			virtual void OnDestroy() override;

			/**
			 * Function inheritated from IComponent,
			 * override for reset the collider
			 */
			virtual void OnReset() override;

			/**
			 * set the collider as default
			 */
			virtual void SetToDefault();

			/**
			 * Function inheritated from IComponent,
			 */
			virtual bool OnStart() override;

			/**
			 * init the save of the shape
			 */
			virtual void InitShapeSave();
			

		public:
			/**
			 * Default constructor
			 */
			Collider() = default;
			/**
			 * Default destructor
			 */
			virtual ~Collider();

			/**
			 * Function inheritated from IComponent,
			 * override for create the collider from the PhysicsScene
			 */
			virtual void OnInit() override;
			/**
			 * Draw the collider
			 * @param cam: the information of the camera
			 * @param shader: the shader to draw with
			 * @param model: the model to draw with
			 */
			virtual void DrawCollider(Core::Datastructure::ICamera* cam, std::shared_ptr<Resources::Shader> shader,
										std::shared_ptr<Resources::Model> model) = 0;
			/**
			 * Destroy the m_pxRigidActor of the collider
			 */
			void DestroyRigidActor();

			/**
			 * destach the shape of the m_pxRigidActor of the collider
			 */
			void DetachShape();

			/**
			 * Create m_pxRigidActor of the collider
			 */
			void CreateRigidActor();

			/**
			 * Init the m_pxRigidActor as an PxRigidDynamic wit the component RigidBody
			 * @param rigidBody: the component RigidBody
			 * @param ID: the ID of the RigidBody for have the Id of the event in the Parent Object
			 * @param pxRigidBody: the PxRigidDynamic of the RigidBody
			 */
			void InitRigidBody(Core::Physics::RigidBody* rigidBody, int& ID, physx::PxRigidDynamic*& pxRigidBody);


			/**
			 * Get the PhysX shape of the collider
			 * @return the PhysX shape of the collider 
			 */
			inline physx::PxShape* GetShape() { return m_pxShape; }

			/**
			 * Get isActive value of the collider
			 * @return if true the collider is active
			 */
			inline bool IsActive() const { return m_isActive; }
			/**
			 * Set isActive value of the collider
			 * @param activate: the bool who set isActive
			 * if true the collider is active
			 */
			void SetActivateCollider(bool activate);

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
			virtual void TriggerShape();

			/**
			 * Set the state simulation of the shape
			 */
			virtual void SimulationShape();

			/**
			 * Set the raycast filter of the shape
			 * @param filter: the filter we to give to the shape
			 */
			virtual void SetRaycastFilter(EFilterRaycast filter);
			/**
			 * Get the raycast filter of the shape
			 * @return the current filter of the shape
			 */
			virtual EFilterRaycast GetRaycastFilter() const;

			/*
			 * Destroy the shape and his pxMaterial
			 */
			virtual void DestroyShape();

			/**
			 * Function cal as an event for set the position of the collider
			 * compared to the pasition of his parent
			 */
			virtual void SetPhysicsTransformParent();


			REGISTER_CLASS(Core::Datastructure::IComponent)
		};
	}
}
