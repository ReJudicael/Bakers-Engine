#include "RigidBody.h"
#include "PxScene.h"
#include "PxRigidStatic.h"
#include "PxPhysics.h"

#include "RootObject.hpp"
#include "PhysicsScene.h"
#include "EngineCore.h"
#include "Maths.hpp"
#include "CapsuleCollider.h"

namespace Core
{
	namespace Physics
	{
		RTTR_PLUGIN_REGISTRATION
		{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
			Datastructure::RegisterDefaultClassConstructor<RigidBody>("Rigidbody");
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Velocity", &RigidBody::GetVelocity, &RigidBody::SetLinearVelocity);
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Mass", &RigidBody::GetMass, &RigidBody::SetMass);
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Use Gravity", &RigidBody::GetUseGravity, &RigidBody::SetUseGravity);
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Rotation XLock", &RigidBody::GetPhysicsLockXRotation, &RigidBody::SetPhysicsLockXRotation);
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Rotation YLock", &RigidBody::GetPhysicsLockYRotation, &RigidBody::SetPhysicsLockYRotation);
			Datastructure::RegisterClassPropertyGS<RigidBody>("Rigidbody", "Rotation ZLock", &RigidBody::GetPhysicsLockZRotation, &RigidBody::SetPhysicsLockZRotation);
			Datastructure::RegisterClassMethod<RigidBody>("Rigidbody", "AddVelocity", &RigidBody::AddVelocity);
			Datastructure::RegisterClassMethod<RigidBody>("Rigidbody", "SetAngularVelocity", &RigidBody::SetAngularVelocity);
			Datastructure::RegisterClassMethod<RigidBody>("Rigidbody", "GetAngularVelocity", &RigidBody::GetAngularVelocity);
		}

		void RigidBody::OnInit()
		{
			// Try to find the collider attached to the Object
			std::list<Core::Physics::Collider*> colliders;
			GetParent()->GetComponentsOfBaseTypeInObject(colliders);

			if (colliders.size() > 0)
				// If one collider is found, init the collider as a RigidBody
				(*colliders.begin())->InitRigidBody(this, m_IDFunctionSetTRS, m_pxRigidBody);

			Core::Datastructure::IUpdatable::OnInit();
		}

		bool RigidBody::OnStart()
		{
			if (!m_pxRigidBody)
				OnInit();
			return IUpdatable::OnStart() && m_pxRigidBody;
		}

		void RigidBody::StartCopy(IComponent*& copyTo) const
		{
			ZoneScoped
			copyTo = new RigidBody();
			OnCopy(copyTo);

		}

		void RigidBody::OnCopy(IComponent* copyTo) const
		{
			ZoneScoped
			ComponentBase::OnCopy(copyTo);
			IUpdatable::OnCopy(copyTo);

			RigidBody* phy = dynamic_cast<RigidBody*>(copyTo);

			phy->m_tmpRigidBodySave = SaveRigidBody();

		}

		RigidBodySave* RigidBody::SaveRigidBody() const
		{
			if (!m_pxRigidBody)
				return nullptr;
			RigidBodySave* save = new RigidBodySave();

			save->Gravity = GetUseGravity();
			save->Mass = GetMass();
			save->XLock = GetPhysicsLockXRotation();
			save->YLock = GetPhysicsLockYRotation();
			save->ZLock = GetPhysicsLockZRotation();

			return save;
		}

		void RigidBody::OnDestroy()
		{
			ComponentBase::OnDestroy();
			IUpdatable::OnDestroy();
			std::list<Core::Physics::Collider*> colliders;
			GetParent()->GetComponentsOfBaseTypeInObject(colliders);

			if (colliders.size() > 0 && !(*colliders.begin())->IsDestroyed())
				(*colliders.begin())->CreateRigidActor();

			GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
		}

		void RigidBody::OnReset()
		{
			
			SetToDefault();
			ComponentBase::OnReset();
			IUpdatable::OnReset();
		}

		void RigidBody::SetToDefault()
		{
			PhysicsLockRotation(false, false, false);
			SetMass(1.f);
			SetUseGravity(true);
		}

		void RigidBody::InitPhysic()
		{
			if (m_tmpRigidBodySave)
			{
				PhysicsLockRotation(m_tmpRigidBodySave->XLock, m_tmpRigidBodySave->YLock, m_tmpRigidBodySave->ZLock);
				SetMass(m_tmpRigidBodySave->Mass);
				SetUseGravity(m_tmpRigidBodySave->Gravity);

				delete m_tmpRigidBodySave;
				m_tmpRigidBodySave = nullptr;
			}

		}

		void RigidBody::OnUpdate(float deltaTime)
		{
			if (!m_pxRigidBody)
			{
				GetParent()->DeleteAnEventTransformChange(m_IDFunctionSetTRS);
			}
			if (m_pxRigidBody->isSleeping())
				return;

			physx::PxVec3 pos = m_pxRigidBody->getGlobalPose().p;
			physx::PxQuat rot = m_pxRigidBody->getGlobalPose().q;

			m_BodyChangeGlobalPos = true;
			GetParent()->SetGlobalPos({ pos.x, pos.y, pos.z });
			m_BodyChangeGlobalPos = true;
			GetParent()->SetGlobalRot({ rot.w, rot.x, rot.y, rot.z });
		}

		void RigidBody::SetPhysicsTransformParent()
		{
			if (m_pxRigidBody == nullptr || m_BodyChangeGlobalPos)
			{
				m_BodyChangeGlobalPos = false;
				return;
			}

			Maths::Vec3 vec = GetParent()->GetGlobalPos();
			physx::PxVec3 position = physx::PxVec3{ vec.x, vec.y, vec.z };
			Maths::Quat quat = GetParent()->GetGlobalRot();
			physx::PxQuat rotation = physx::PxQuat{ quat.x, quat.y, quat.z, quat.w };

			m_pxRigidBody->setGlobalPose(physx::PxTransform(position, rotation));
		}

		void RigidBody::SetLinearVelocity(Core::Maths::Vec3 newVelocity)
		{
			if (!m_pxRigidBody)
				return;
			m_pxRigidBody->setLinearVelocity({ newVelocity.x, newVelocity.y, newVelocity.z });
		}

		Core::Maths::Vec3 RigidBody::GetVelocity() const
		{
			if (!m_pxRigidBody)
				return { 0.f, 0.f, 0.f };
			physx::PxVec3 vec{ m_pxRigidBody->getLinearVelocity() };
			return { vec.x, vec.y, vec.z };
		}

		void RigidBody::SetAngularVelocity(Core::Maths::Vec3 angularVelocity)
		{
			if (!m_pxRigidBody)
				return;
			m_pxRigidBody->setAngularVelocity({ angularVelocity.x, angularVelocity.y, angularVelocity.z });
		}

		Core::Maths::Vec3 RigidBody::GetAngularVelocity() const
		{
			if (!m_pxRigidBody)
				return { 0.f, 0.f, 0.f };
			physx::PxVec3 vec{ m_pxRigidBody->getAngularVelocity() };
			return { vec.x, vec.y, vec.z };
		}

		void RigidBody::AddVelocity(const Core::Maths::Vec3 vector)
		{
			m_pxRigidBody->addForce({ vector.x,vector.y,vector.z }, physx::PxForceMode::eVELOCITY_CHANGE);
		}

		void RigidBody::SetMass(const float mass)
		{
			// if there is already a shape put the value in the shape
			// if the object isn't going to be destroyed save the value
			if (m_pxRigidBody)
				m_pxRigidBody->setMass(static_cast<physx::PxReal>(mass));
			else if (!IsDestroyed())
			{
				if (!m_tmpRigidBodySave)
					m_tmpRigidBodySave = new RigidBodySave();
				m_tmpRigidBodySave->Mass = mass;
			}
		}

		float RigidBody::GetMass() const
		{
			if (m_pxRigidBody == nullptr)
			{
				if (m_tmpRigidBodySave)
					return m_tmpRigidBodySave->Mass;
				return 0;
			}
			return m_pxRigidBody->getMass();
		}

		void RigidBody::ClearForces()
		{
			if (m_pxRigidBody)
				return;
			m_pxRigidBody->clearForce();
			m_pxRigidBody->clearTorque();
		}

		void RigidBody::PhysicsLockRotation(bool Axisx, bool Axisy, bool Axisz)
		{
			SetPhysicsLockXRotation(Axisx);
			SetPhysicsLockYRotation(Axisy);
			SetPhysicsLockZRotation(Axisz);
		}


		void RigidBody::SetPhysicsLockXRotation(bool Axisx)
		{
			// if there is already a shape put the value in the shape
			// if the object isn't going to be destroyed save the value
			if (m_pxRigidBody)
				m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, Axisx);
			else if (!IsDestroyed())
			{
				if (!m_tmpRigidBodySave)
					m_tmpRigidBodySave = new RigidBodySave();
				m_tmpRigidBodySave->XLock = Axisx;
			}
		}

		bool RigidBody::GetPhysicsLockXRotation() const
		{
			if (m_pxRigidBody == nullptr)
			{
				if (m_tmpRigidBodySave)
					return m_tmpRigidBodySave->XLock;
				return false;
			}
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);
		}

		void RigidBody::SetPhysicsLockYRotation(bool Axisy)
		{
			// if there is already a shape put the value in the shape
			// if the object isn't going to be destroyed save the value
			if (m_pxRigidBody)
				m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, Axisy);
			else if (!IsDestroyed())
			{
				if (!m_tmpRigidBodySave)
					m_tmpRigidBodySave = new RigidBodySave();
				m_tmpRigidBodySave->YLock = Axisy;
			}
		}

		bool RigidBody::GetPhysicsLockYRotation() const
		{
			if (m_pxRigidBody == nullptr)
			{
				if (m_tmpRigidBodySave)
					return m_tmpRigidBodySave->YLock;
				return false;
			}
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y);
		}

		void RigidBody::SetPhysicsLockZRotation(bool Axisz)
		{
			if (m_pxRigidBody)
				m_pxRigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, Axisz);
			else if (!IsDestroyed())
			{
				if (!m_tmpRigidBodySave)
					m_tmpRigidBodySave = new RigidBodySave();
				m_tmpRigidBodySave->ZLock = Axisz;
			}
		}

		bool RigidBody::GetPhysicsLockZRotation() const
		{
			if (m_pxRigidBody == nullptr)
			{
				if (m_tmpRigidBodySave)
					return m_tmpRigidBodySave->ZLock;
				return false;
			}
			return m_pxRigidBody->getRigidDynamicLockFlags().isSet(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
		}

		void RigidBody::SetUseGravity(bool use)
		{
			// if there is already a shape put the value in the shape
			// if the object isn't going to be destroyed save the value
			if (m_pxRigidBody)
				m_pxRigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !use);
			else if (!IsDestroyed())
			{
				if (!m_tmpRigidBodySave)
					m_tmpRigidBodySave = new RigidBodySave();
				m_tmpRigidBodySave->Gravity = use;
			}
		}

		bool RigidBody::GetUseGravity() const
		{
			if (!m_pxRigidBody)
			{
				if (m_tmpRigidBodySave)
					return m_tmpRigidBodySave->Gravity;
				return false;
			}
			return !m_pxRigidBody->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
		}

		RigidBody::~RigidBody()
		{
			if (m_tmpRigidBodySave)
				delete m_tmpRigidBodySave;
		}
	}
}