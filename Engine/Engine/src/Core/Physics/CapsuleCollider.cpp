#include "CapsuleCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"
#include "ICamera.h"
#include "Shader.h"
#include "Mat4.hpp"
#include "Transform.hpp"
#include "ResourcesManager.h"
#include "PhysicsScene.h"
#include "OffsetMesh.h"
#include "RootObject.hpp"
#include "EngineCore.h"

namespace Core::Physics
{

	RTTR_PLUGIN_REGISTRATION
	{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
		registration::class_<Core::Physics::CapsuleCollider>("Capsule Collider")
		.constructor()
		.property("Capsule Half Extent", &Core::Physics::CapsuleCollider::GetCapsuleHalfExtent ,&Core::Physics::CapsuleCollider::SetCapsuleHalfExtent);
	}

	void CapsuleCollider::OnInit()
	{
		Core::Physics::Collider::OnInit();
		GetRoot()->GetEngine()->PutCapsuleCollider(this);
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
			col->m_tmpColliderSave->extent = { GetCapsuleHalfExtent().x, GetCapsuleHalfExtent().y , 0.f};
	}

	void CapsuleCollider::OnDestroy()
	{
		GetRoot()->GetEngine()->DeleteCapsuleCollider(this);
		Core::Physics::Collider::OnDestroy();
	}

	void CapsuleCollider::OnReset()
	{
		SetToDefault();
		Core::Physics::Collider::OnReset();
		Core::Datastructure::ComponentBase::OnReset();
	}

	void CapsuleCollider::SetToDefault()
	{
		Collider::SetToDefault();
		SetCapsuleHalfExtent({ 0.5f, 0.5f });
	}

	void CapsuleCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);
		// rotate 90 degree on z axis
		physx::PxQuat localRotation = physx::PxQuat(1.57f, physx::PxVec3{ 0.f, 0.f, 1.f });
		physx::PxVec3 mat = { 1.5f, 1.5f, 0.0f };
		physx::PxReal radius{ 0.5 };
		physx::PxReal halfHeight{ 0.5 };

		if (m_tmpColliderSave)
		{
			radius = m_tmpColliderSave->extent.x;
			halfHeight = m_tmpColliderSave->extent.y;
		}

		m_pxMaterial = physics->createMaterial(mat.x, mat.y, mat.z);
		m_pxShape = physics->createShape(physx::PxCapsuleGeometry(radius, halfHeight), *m_pxMaterial, true);
		SetRaycastFilter(Core::Physics::EFilterRaycast::GROUPE1);
	}

	void CapsuleCollider::SetCapsuleHalfExtent(Core::Maths::Vec2 halfExtent)
	{
		if (halfExtent.x <= 0.f || halfExtent.y <= 0.f)
			return;

		if (m_pxShape)
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

	void CapsuleCollider::DrawCollider(Core::Datastructure::ICamera* cam, std::shared_ptr<Resources::Shader> shader,
		std::shared_ptr<Resources::Model> model)
	{
		glEnable(GL_DEPTH_TEST);

		if (model == nullptr || model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
			return;

		glBindVertexArray(model->VAOModel);

		shader->UseProgram();

		Core::Maths::Vec3 localPos{ m_pxShape->getLocalPose().p.x, m_pxShape->getLocalPose().p.y, m_pxShape->getLocalPose().p.z };
		Core::Maths::Quat localRot{ m_pxShape->getLocalPose().q.w, m_pxShape->getLocalPose().q.x, m_pxShape->getLocalPose().q.y, m_pxShape->getLocalPose().q.z };
		Core::Datastructure::Transform Ltrs;
		Ltrs.SetLocalPos(localPos);
		Ltrs.SetLocalRot(localRot);
		Ltrs.SetLocalScale({ GetCapsuleHalfExtent().x, GetCapsuleHalfExtent().y, GetCapsuleHalfExtent().x });

		Core::Datastructure::Transform Gtrs;
		Gtrs.SetLocalPos(GetParent()->GetGlobalPos());
		Gtrs.SetLocalRot(GetParent()->GetGlobalRot());
		Gtrs.SetLocalScale({ 1.f,1.f,1.f });

		glUniformMatrix4fv(shader->GetLocation("uModel"), 1, GL_TRUE, (Gtrs.GetLocalTrs() * Ltrs.GetLocalTrs()).array);
		glUniformMatrix4fv(shader->GetLocation("uCam"), 1, GL_TRUE, cam->GetCameraMatrix().array);
		glUniformMatrix4fv(shader->GetLocation("uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().array);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, model->offsetsMesh[0].count, GL_UNSIGNED_INT,
			(GLvoid*)(model->offsetsMesh[0].beginIndices * sizeof(GLuint)));

		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}