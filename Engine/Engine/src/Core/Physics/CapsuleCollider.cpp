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

namespace Core::Physics
{

	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::CapsuleCollider>("Collider")
		.property("Capsule Half Extent", &Core::Physics::CapsuleCollider::GetCapsuleHalfExtent ,&Core::Physics::CapsuleCollider::SetCapsuleHalfExtent)
		;
	}

	CapsuleCollider::CapsuleCollider(Resources::Loader::ResourcesManager* resources) :
		Collider(resources)
	{
		InitModel(resources->GetModel("Capsule"));
	}

	void CapsuleCollider::CreateShape(physx::PxPhysics* physics)
	{
		//physx::PxVec2 extent = physx::PxVec3(m_extent.x, m_extent.y);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);
		// rotation 90 on z axis
		physx::PxQuat localRot = physx::PxQuat(1.57f, physx::PxVec3{0.f, 0.f, 1.f});

		m_pxMaterial = physics->createMaterial(1.f, 1.f, 0.0f);
		m_pxShape = physics->createShape(physx::PxCapsuleGeometry(0.5f,0.5f), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition, localRot));

		SetRaycastFilter(Core::Physics::EFilterRaycast::GROUPE1);
	}

	void CapsuleCollider::SetCapsuleHalfExtent(Core::Maths::Vec2 halfExtent)
	{
		m_extent = halfExtent;
		if (m_pxShape != nullptr)
			m_pxShape->setGeometry(physx::PxCapsuleGeometry(halfExtent.x, halfExtent.y));
	}

	Core::Maths::Vec2 CapsuleCollider::GetCapsuleHalfExtent()
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

	void CapsuleCollider::DrawCollider(Core::Datastructure::ICamera* cam, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot)
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
		Gtrs.SetLocalPos(pos);
		Gtrs.SetLocalRot(rot);
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