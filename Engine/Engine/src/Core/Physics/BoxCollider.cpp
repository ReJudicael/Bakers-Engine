//#include <GLFW/glfw3.h>
#include "BoxCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"
#include "ICamera.h"
#include "Shader.h"
#include "Mat4.hpp"
#include "Transform.hpp"
#include "PhysicsScene.h"
#include "OffsetMesh.h"
#include "LoadResources.h"
#include "RootObject.hpp"
#include "EngineCore.h"

namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		ZoneScopedN("Registering RTTR")
		registration::class_<Core::Physics::BoxCollider>("Box Collider")
		.constructor()
		.property("Box Half Extent", &Core::Physics::BoxCollider::GetBoxHalfExtent ,&Core::Physics::BoxCollider::SetBoxHalfExtent)
		;
	}

	void BoxCollider::OnInit()
	{
		Core::Physics::Collider::OnInit();
		GetRoot()->GetEngine()->PutBoxCollider(this);
		Core::Datastructure::ComponentBase::OnInit();
	}

	bool BoxCollider::OnStart()
	{
		return Core::Physics::Collider::OnStart();
	}
	void BoxCollider::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new BoxCollider();
		OnCopy(copyTo);
	}

	void BoxCollider::OnCopy(IComponent* copyTo) const
	{
		Core::Physics::Collider::OnCopy(copyTo);

		BoxCollider* col = dynamic_cast<BoxCollider*>(copyTo);
		if(col->m_tmpColliderSave)
			col->m_tmpColliderSave->extent = GetBoxHalfExtent();
	}

	void BoxCollider::OnDestroy()
	{
		GetRoot()->GetEngine()->DeleteBoxCollider(this);
		Core::Physics::Collider::OnDestroy();
	}

	void BoxCollider::OnReset()
	{
		SetToDefault();
		Core::Physics::Collider::OnReset();
		Core::Datastructure::ComponentBase::OnReset();
	}

	void BoxCollider::SetToDefault()
	{
		Collider::SetToDefault();
		SetBoxHalfExtent({ 0.5f, 0.5f, 0.5f });
	}

	void BoxCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);
		physx::PxQuat localRotation{};
		physx::PxVec3 mat = { 1.5f, 1.5f, 0.0f };
		physx::PxVec3 extent{ 0.5 };

		if (m_tmpColliderSave)
			extent = { m_tmpColliderSave->extent.x, m_tmpColliderSave->extent.y, m_tmpColliderSave->extent.z };

		m_pxMaterial = physics->createMaterial(mat.x, mat.y, mat.z);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial, true);
	}

	void BoxCollider::SetBoxHalfExtent(Core::Maths::Vec3 halfExtent)
	{
		if (halfExtent.x <= 0.f || halfExtent.y <= 0.f || halfExtent.z <= 0.f)
			return;

		if(m_pxShape)
			m_pxShape->setGeometry(physx::PxBoxGeometry(physx::PxVec3{ halfExtent.x, halfExtent.y, halfExtent.z }));
		else if (!IsDestroyed())
		{
			if (!m_tmpColliderSave)
				m_tmpColliderSave = new ColliderSave();
			m_tmpColliderSave->extent = halfExtent;
		}
	}
	
	Core::Maths::Vec3 BoxCollider::GetBoxHalfExtent() const
	{
		if (m_pxShape != nullptr)
		{
			physx::PxBoxGeometry box;
			if (m_pxShape->getBoxGeometry(box))
				return { box.halfExtents.x, box.halfExtents.y, box.halfExtents.z };

			return Core::Maths::Vec3();
		}
		return Core::Maths::Vec3();

	}


	void BoxCollider::DrawCollider(Core::Datastructure::ICamera* cam, std::shared_ptr<Resources::Shader> shader,
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
		Ltrs.SetLocalScale(GetBoxHalfExtent() * 2);

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