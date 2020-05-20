#include "SphereCollider.h"
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

namespace Core::Physics
{

	RTTR_PLUGIN_REGISTRATION
	{
		ZoneScopedN("Registering RTTR")
		registration::class_<Core::Physics::SphereCollider>("Collider")
		.property("Radius", &Core::Physics::SphereCollider::GetSphereHalfExtent ,&Core::Physics::SphereCollider::SetSphereHalfExtent)
		;
	}

	SphereCollider::SphereCollider(Resources::Loader::ResourcesManager* resources) :
		Collider(resources)
	{
		InitModel(resources->GetModel("Sphere"));
	}

	void SphereCollider::CreateShape(physx::PxPhysics* physics)
	{
		//physx::PxVec2 extent = physx::PxVec3(m_extent.x, m_extent.y);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);

		m_pxMaterial = physics->createMaterial(1.5f, 1.5f, 0.0f);
		m_pxShape = physics->createShape(physx::PxSphereGeometry(0.5f), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));

		SetRaycastFilter(Core::Physics::EFilterRaycast::GROUPE1);
	}

	void SphereCollider::SetSphereHalfExtent(float halfExtent)
	{
		m_extent = halfExtent;
		if (m_pxShape != nullptr)
			m_pxShape->setGeometry(physx::PxSphereGeometry(halfExtent));
	}

	float SphereCollider::GetSphereHalfExtent()
	{
		if (m_pxShape != nullptr)
		{
			physx::PxSphereGeometry sphere;
			if (m_pxShape->getSphereGeometry(sphere))
				return sphere.radius;

			return 0.f;
		}
		return 0.f;

	}

	void SphereCollider::DrawCollider(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot)
	{

		glEnable(GL_DEPTH_TEST);

		if (m_model == nullptr || m_model->stateVAO != Resources::EOpenGLLinkState::ISLINK)
			return;

		glBindVertexArray(m_model->VAOModel);

		m_shader->UseProgram();

		Core::Maths::Vec3 localPos{ m_pxShape->getLocalPose().p.x, m_pxShape->getLocalPose().p.y, m_pxShape->getLocalPose().p.z };
		Core::Maths::Quat localRot{ m_pxShape->getLocalPose().q.w, m_pxShape->getLocalPose().q.x, m_pxShape->getLocalPose().q.y, m_pxShape->getLocalPose().q.z };
		Core::Datastructure::Transform Ltrs;
		Ltrs.SetLocalPos(localPos);
		Ltrs.SetLocalRot(localRot);
		Ltrs.SetLocalScale({ GetSphereHalfExtent(), GetSphereHalfExtent(), GetSphereHalfExtent() });

		Core::Datastructure::Transform Gtrs;
		Gtrs.SetLocalPos(pos);
		Gtrs.SetLocalRot(rot);
		Gtrs.SetLocalScale({ 1.f,1.f,1.f });

		glUniformMatrix4fv(m_shader->GetLocation("uModel"), 1, GL_TRUE, (Ltrs.GetLocalTrs() * Gtrs.GetLocalTrs()).array);
		glUniformMatrix4fv(m_shader->GetLocation("uCam"), 1, GL_TRUE, view.array);
		glUniformMatrix4fv(m_shader->GetLocation("uProj"), 1, GL_FALSE, proj.array);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, m_model->offsetsMesh[0].count, GL_UNSIGNED_INT,
			(GLvoid*)(m_model->offsetsMesh[0].beginIndices * sizeof(GLuint)));

		glBindVertexArray(0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}