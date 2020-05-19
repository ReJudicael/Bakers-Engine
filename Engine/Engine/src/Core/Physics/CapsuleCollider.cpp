#include "CapsuleCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"
#include "ICamera.h"
#include "Shader.h"
#include "Mat4.hpp"
#include "Transform.hpp"
#include "LoadResources.h"
#include "PhysicsScene.h"
#include "OffsetMesh.h"
#include "RootObject.hpp"
#include "EngineCore.h"

namespace Core::Physics
{

	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::CapsuleCollider>("Capsule Collider")
		.constructor()
		.property("Capsule Half Extent", &Core::Physics::CapsuleCollider::GetCapsuleHalfExtent ,&Core::Physics::CapsuleCollider::SetCapsuleHalfExtent)
		;
	}

	void CapsuleCollider::OnInit()
	{
		Core::Datastructure::RootObject* root = GetRoot();
		InitShader(root->GetEngine()->GetResourcesManager()->GetShader("Wireframe"));
		InitModel(root->GetEngine()->GetResourcesManager()->GetModel("Capsule"));
		Core::Physics::Collider::OnInit();
		Core::Datastructure::ComponentBase::OnInit();
	}

	bool CapsuleCollider::OnStart()
	{
		return Core::Physics::Collider::OnStart();
	}
	void CapsuleCollider::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new CapsuleCollider();
		OnCopy(copyTo);
	}

	void CapsuleCollider::OnCopy(IComponent* copyTo) const
	{
		Core::Physics::Collider::OnCopy(copyTo);

		CapsuleCollider* col = dynamic_cast<CapsuleCollider*>(copyTo);

		if(col->m_tmpColliderSave)
			col->m_tmpColliderSave->extent = { GetCapsuleHalfExtent().x, GetCapsuleHalfExtent().y };
	}

	void CapsuleCollider::OnDestroy()
	{
		Core::Physics::Collider::OnDestroy();
	}

	void CapsuleCollider::OnReset()
	{
		Core::Physics::Collider::Reset();
	}

	void CapsuleCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);
		// rotate 90 degree on z axis
		physx::PxQuat localRotation = physx::PxQuat(1.57f, physx::PxVec3{ 0.f, 0.f, 1.f });
		physx::PxVec3 mat = { 1.5f, 1.5f, 0.0f };
		physx::PxReal radius{ 0.5 };
		physx::PxReal halfHeight{ 0.5 };
		physx::PxU32 filter;

		if (m_tmpColliderSave)
		{
			localPosition = { m_tmpColliderSave->localPosition.x, m_tmpColliderSave->localPosition.y, m_tmpColliderSave->localPosition.z };
			localRotation = { m_tmpColliderSave->localRotation.x, m_tmpColliderSave->localRotation.y,
								m_tmpColliderSave->localRotation.z, m_tmpColliderSave->localRotation.w };
			radius = m_tmpColliderSave->extent.x;
			halfHeight = m_tmpColliderSave->extent.y;

			mat = { m_tmpColliderSave->physicsMaterial.x, m_tmpColliderSave->physicsMaterial.y, m_tmpColliderSave->physicsMaterial.z };
		}

		m_pxMaterial = physics->createMaterial(mat.x, mat.y, mat.z);

		m_pxShape = physics->createShape(physx::PxCapsuleGeometry(radius, halfHeight), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition, localRotation));


		if (m_tmpColliderSave)
		{
			SetRaycastFilter(m_tmpColliderSave->raycastFilter);
			Trigger(m_tmpColliderSave->isTrigger);
			delete m_tmpColliderSave;
			m_tmpColliderSave = nullptr;
		}
		else
			SetRaycastFilter(Core::Physics::EFilterRaycast::GROUPE1);


	}

	void CapsuleCollider::SetCapsuleHalfExtent(Core::Maths::Vec2 halfExtent)
	{
		if (m_pxShape && halfExtent.x > 0.f && halfExtent.y > 0.f)
			m_pxShape->setGeometry(physx::PxCapsuleGeometry(halfExtent.x, halfExtent.y));
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->extent = {halfExtent.x, halfExtent.y, 0.f};
		}

	}

	Core::Maths::Vec2 CapsuleCollider::GetCapsuleHalfExtent() const
	{
		if (m_pxShape != nullptr)
		{
			physx::PxCapsuleGeometry caps;
			if (m_pxShape->getCapsuleGeometry(caps))
				return { caps.radius, caps.halfHeight };

			return Core::Maths::Vec2();
		}
		return Core::Maths::Vec2();

	}

	void CapsuleCollider::OnDraw(Core::Datastructure::ICamera* cam)
	{

		glEnable(GL_DEPTH_TEST);

		if (m_model == nullptr || m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
			return;

		glBindVertexArray(m_model->VAOModel);

		m_shader->UseProgram();

		Core::Maths::Vec3 localPos{ m_pxShape->getLocalPose().p.x, m_pxShape->getLocalPose().p.y, m_pxShape->getLocalPose().p.z };
		Core::Maths::Quat localRot{ m_pxShape->getLocalPose().q.w, m_pxShape->getLocalPose().q.x, m_pxShape->getLocalPose().q.y, m_pxShape->getLocalPose().q.z };
		Core::Datastructure::Transform Ltrs;
		Ltrs.SetLocalPos(localPos + Core::Maths::Vec3{0.f,-0.5f,0.f});
		Ltrs.SetLocalRot(localRot * Core::Maths::Quat(0.7071068f, 0, 0, -0.7071068f));
		Ltrs.SetLocalScale({ GetCapsuleHalfExtent().x, GetCapsuleHalfExtent().y, GetCapsuleHalfExtent().x });

		Core::Datastructure::Transform Gtrs;
		Gtrs.SetLocalPos(GetParent()->GetGlobalPos());
		Gtrs.SetLocalRot(GetParent()->GetGlobalRot());
		Gtrs.SetLocalScale({ 1.f,1.f,1.f });

		glUniformMatrix4fv(m_shader->GetLocation("uModel"), 1, GL_TRUE, (Gtrs.GetLocalTrs() * Ltrs.GetLocalTrs()).array);
		glUniformMatrix4fv(m_shader->GetLocation("uCam"), 1, GL_TRUE, cam->GetCameraMatrix().array);
		glUniformMatrix4fv(m_shader->GetLocation("uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().array);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, m_model->offsetsMesh[0].count, GL_UNSIGNED_INT,
			(GLvoid*)(m_model->offsetsMesh[0].beginIndices * sizeof(GLuint)));

		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}