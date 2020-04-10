//#include <GLFW/glfw3.h>
#include "BoxCollider.h"
#include "Collider.h"
#include "Vec3.hpp"
#include "PxPhysics.h"
#include "ICamera.h"
#include "Shader.h"
#include "Mat4.hpp"
#include "Transform.hpp"
#include "OffsetMesh.h"

namespace Core::Physics
{
	RTTR_PLUGIN_REGISTRATION
	{
		registration::class_<Core::Physics::BoxCollider>("Collider")
		.property("Box Half Extent", &Core::Physics::BoxCollider::GetBoxHalfExtent ,&Core::Physics::BoxCollider::SetBoxHalfExtent)
		;
	}

	void BoxCollider::CreateShape(physx::PxPhysics* physics)
	{
		physx::PxVec3 extent = physx::PxVec3(m_extent.x, m_extent.y, m_extent.z);
		physx::PxVec3 localPosition = physx::PxVec3(0.f, 0.f, 0.f);

		m_pxMaterial = physics->createMaterial(1.5f, 1.5f, 0.0f);
		m_pxShape = physics->createShape(physx::PxBoxGeometry(extent), *m_pxMaterial, true);
		m_pxShape->setLocalPose(physx::PxTransform(localPosition));

		/*float centerPosX = 0.f;
		float centerPosY = 0.f;
		float centerPosZ = 0.f;
		physx::PxBoxGeometry box;
		m_pxShape->getBoxGeometry(box);

		float halfSideLengthX = 0.5;
		float halfSideLengthY = 0.5;
		float halfSideLengthZ = 0.5;

		GLfloat vertices[] =
		{
			// front face
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom right
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

			// back face
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top left
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom left

			// left face
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

			// right face
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

			// top face
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
			centerPosX - halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
			centerPosX + halfSideLengthX, centerPosY + halfSideLengthY, centerPosZ + halfSideLengthZ, // bottom left

			// top face
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ, // top left
			centerPosX - halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // top right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ - halfSideLengthZ, // bottom right
			centerPosX + halfSideLengthX, centerPosY - halfSideLengthY, centerPosZ + halfSideLengthZ  // bottom left
		};

		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		GLuint VBO{ 0 };
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * GL_FLOAT, (void*)0);

		m_VAO = VAO;
		glBindVertexArray(0);*/
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

	}

	void BoxCollider::DrawCollider(Core::Datastructure::ICamera* cam, const Core::Maths::Vec3& pos, const Core::Maths::Quat& rot)
	{
		
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(m_model->VAOModel);

		m_shader->UseProgram();

		Core::Maths::Vec3 localPos{ m_pxShape->getLocalPose().p.x, m_pxShape->getLocalPose().p.y, m_pxShape->getLocalPose().p.z };
		Core::Maths::Quat localRot{ m_pxShape->getLocalPose().q.w, m_pxShape->getLocalPose().q.x, m_pxShape->getLocalPose().q.y, m_pxShape->getLocalPose().q.z };
		Core::Datastructure::Transform Ltrs;
		Ltrs.SetLocalPos(localPos);
		Ltrs.SetLocalRot(localRot);
		Ltrs.SetLocalScale(GetBoxHalfExtent() * 2);

		Core::Datastructure::Transform Gtrs;
		Gtrs.SetLocalPos(pos);
		Gtrs.SetLocalRot(rot);
		Gtrs.SetLocalScale({ 1.f,1.f,1.f });

		glUniformMatrix4fv(m_shader->GetLocation("uModel"), 1, GL_TRUE, (Ltrs.GetLocalTrs() * Gtrs.GetLocalTrs()).m_array);
		glUniformMatrix4fv(m_shader->GetLocation("uCam"), 1, GL_TRUE, cam->GetCameraMatrix().m_array);
		glUniformMatrix4fv(m_shader->GetLocation("uProj"), 1, GL_FALSE, cam->GetPerspectiveMatrix().m_array);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawElements(GL_TRIANGLES, m_model->offsetsMesh[0].count, GL_UNSIGNED_INT,
			(GLvoid*)(m_model->offsetsMesh[0].beginIndices * sizeof(GLuint)));


		glBindVertexArray(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}