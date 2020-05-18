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
		registration::class_<Core::Physics::BoxCollider>("Box Collider")
		.constructor()
		.property("Box Half Extent", &Core::Physics::BoxCollider::GetBoxHalfExtent ,&Core::Physics::BoxCollider::SetBoxHalfExtent)
		;
	}

	void BoxCollider::OnInit()
	{
		Core::Datastructure::RootObject* root = GetRoot();
		InitShader(root->GetEngine()->GetResourcesManager()->GetShader("Wireframe"));
		InitModel(root->GetEngine()->GetResourcesManager()->GetModel("Cube"));
		Core::Physics::Collider::OnInit();
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
	}

	void BoxCollider::OnDestroy()
	{
		Core::Physics::Collider::OnDestroy();
	}

	void BoxCollider::OnReset()
	{
		Core::Physics::Collider::Reset();
	}

	void BoxCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 extent = physx::PxVec3(m_extent.x, m_extent.y, m_extent.z);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);

		m_pxMaterial = physics->createMaterial(1.5f, 1.5f, 0.0f);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));

		SetRaycastFilter(Core::Physics::EFilterRaycast::GROUPE1);
	}

	void BoxCollider::SetBoxHalfExtent(Core::Maths::Vec3 halfExtent)
	{
		m_extent = halfExtent;
		if(m_pxShape != nullptr)
			m_pxShape->setGeometry(physx::PxBoxGeometry(physx::PxVec3{ halfExtent.x, halfExtent.y, halfExtent.z }));
	}
	
	Core::Maths::Vec3 BoxCollider::GetBoxHalfExtent()
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

	void BoxCollider::OnDraw(Core::Datastructure::ICamera* cam)
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
		Ltrs.SetLocalScale(GetBoxHalfExtent() * 2);

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