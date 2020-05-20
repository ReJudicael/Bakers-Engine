#include <iostream>

#include "PlayerCamera.h"
#include "Object.hpp"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "InputSystem.hpp"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	registration::class_<Core::Renderer::PlayerCamera>("Player Camera")
		.constructor()
		.constructor<const float, const float, const float, const float, const float>()
		.property_readonly("Is running", &Core::Renderer::PlayerCamera::m_isRunning)
		.property("Speed", &Core::Renderer::PlayerCamera::m_speed)
		.property("Running speed", &Core::Renderer::PlayerCamera::m_runningSpeed)
		.property("Min move length", &Core::Renderer::PlayerCamera::m_minMoveLength)
		.property("Move lerp speed", &Core::Renderer::PlayerCamera::m_moveLerpSpeed)
		.property("Blur", &Core::Renderer::PlayerCamera::m_blurActivated)
		.property("Black and White", &Core::Renderer::PlayerCamera::m_bwActivated)
		.property("Color custom", &Core::Renderer::PlayerCamera::m_colorActivated)
		.property("Postprocess Color", &Core::Renderer::PlayerCamera::m_color);
}

namespace Core::Renderer
{
	PlayerCamera::PlayerCamera() : Camera()
	{
		ZoneScoped
	}

	PlayerCamera::PlayerCamera(const float width, const float height, const float fov, const float near, const float far) : Camera(width, height, fov, near, far)
	{
		ZoneScoped
	}

	void PlayerCamera::OnInit()
	{
		Core::Datastructure::ICamera::OnInit();

		for (int i{ 0 }; i < 3; ++i)
		{
			m_postProcessFBO.push_back(GetRoot()->GetEngine()->CreateFBO(m_cameraWidth, m_cameraHeight, Core::Renderer::FBOType::POSTPROCESSING));
			m_postProcessFBO[i]->userPtr = this;
		}

		m_postProcessFBO[0]->InitPostProcess(GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcess", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessShader.frag"));
		m_postProcessFBO[1]->InitPostProcess(GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBlur", "Resources\\Shaders\\PostprocessBlurShader.vert", "Resources\\Shaders\\PostprocessBlurShader.frag"));
		m_postProcessFBO[2]->InitPostProcess(GetRoot()->GetEngine()->GetResourcesManager()->CreateShader("PostProcessBW", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessBlackWhiteShader.frag"));
	}

	std::vector<Core::Renderer::Framebuffer*> PlayerCamera::GetActiveFramebuffers()
	{
		std::vector<Core::Renderer::Framebuffer*> activeFBOs;

		for (int i{ 0 }; i < m_postProcessFBO.size(); ++i)
		{
			if (m_postProcessFBO[i]->data.active)
				activeFBOs.push_back(m_postProcessFBO[i]);
		}

		return activeFBOs;
	}

	void PlayerCamera::MoveWithInput()
	{
		if (Input()->IsMouseButtonUnused(EMouseButton::RIGHT))
		{
			Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::DEFAULT);
			m_mousePos = Input()->GetMousePos();
			m_isMouseSet = false;
		}
		else
		{
			Input()->SetMouseAppearance(Core::SystemManagement::ECursorAppearance::INVISIBLE);
			ComputeTranslation();
			ComputeRotation();
		}
	}

	void PlayerCamera::ComputeTranslation()
	{
		Core::Maths::Vec3 move = { 0, 0, 0 };

		if (Input()->IsKeyDown(EKey::W))
			move += m_parent->Forward() * -1;
		if (Input()->IsKeyDown(EKey::A))
			move += m_parent->Right() * -1;
		if (Input()->IsKeyDown(EKey::D))
			move += m_parent->Right();
		if (Input()->IsKeyDown(EKey::S))
			move += m_parent->Forward();

		if (Input()->IsKeyDown(EKey::LEFT_SHIFT))
			m_isRunning = true;
		else if (Input()->IsKeyUp(EKey::LEFT_SHIFT))
			m_isRunning = false;

		if (move.SquaredLength() > 0)
			Move(move);
	}

	void PlayerCamera::ComputeRotation()
	{
		Core::Maths::Vec2 newPos = Input()->GetMousePos();
		Core::Maths::Vec2 mouseMove = m_mousePos - newPos;

		if (mouseMove.SquaredLength() > 0)
		{
			if (m_isMouseSet)
				Rotate({ mouseMove.y, mouseMove.x, 0 });
		}

		Input()->SetMousePos(m_mousePos);
		m_isMouseSet = true;
	}

	void PlayerCamera::OnUpdate(float deltaTime)
	{
		m_postProcessFBO[0]->data.active = m_colorActivated;
		m_postProcessFBO[1]->data.active = m_blurActivated;
		m_postProcessFBO[2]->data.active = m_bwActivated;

		MoveWithInput();

		Camera::OnUpdate(deltaTime);

		UpdatePosition(deltaTime);
		UpdateRotation(deltaTime);
	}

	void PlayerCamera::UpdatePosition(float deltaTime)
	{
		float length = m_movement.SquaredLength();

		if (length == 0) // Do not update transform if no movement has been made
			return;

		if (m_isRunning)
			m_parent->Translate(m_movement * m_runningSpeed * deltaTime);
		else
			m_parent->Translate(m_movement * m_speed * deltaTime);

		// Decrease movement
		m_movement *= m_moveLerpSpeed;

		// Stop movement if it is too small
		if (length < m_minMoveLength)
			m_movement = { 0, 0, 0 };
	}

	void PlayerCamera::UpdateRotation(float deltaTime)
	{
		if (m_isRotating)
		{
			// Increase current rotation with new one
			m_pitch += m_angularSpeed * m_angularMovement.x * deltaTime;
			m_yaw += m_angularSpeed * m_angularMovement.y * deltaTime;

			// Create euler angles vector3 with pitch and yaw rotation
			Core::Maths::Vec3 newRotation(m_pitch, m_yaw, 0);
			Core::Maths::Quat q(newRotation);
			m_parent->SetRot(q);
		}

		// Decrease rotation
		m_angularMovement *= m_angularLerpSpeed;

		// Stop rotation if too small
		if (m_angularMovement.SquaredLength() < m_minAngularLength)
		{
			m_angularMovement = { 0, 0, 0 };
			m_isRotating = false;
		}
	}

	void PlayerCamera::Move(Core::Maths::Vec3 move)
	{
		m_movement = move.Normalized();
	}

	void PlayerCamera::Rotate(Core::Maths::Vec3 move)
	{
		m_angularMovement = move;
		m_isRotating = true;
	}

	void PlayerCamera::Resize(unsigned width, unsigned height)
	{
		Core::Datastructure::ICamera::Resize(width, height);

		for (int i{ 0 }; i < m_postProcessFBO.size(); ++i)
			m_postProcessFBO[i]->Resize(width, height);
	}

	void PlayerCamera::Draw(const std::list<Core::Datastructure::IRenderable*>& renderables)
	{
		ZoneScoped
			ZoneText("Render of a camera", 22)
			TracyGpuZone("Rendering frame buffer")
			if (IsStarted() && m_isActive && !IsDestroyed() && m_parent->IsActive())
			{
				glViewport(m_fbo->Size[0], m_fbo->Size[1], m_fbo->Size[2], m_fbo->Size[3]);
				DrawDepth(renderables);

				std::vector<Core::Renderer::Framebuffer*> FBO = GetActiveFramebuffers();
				if (FBO.size() > 0)
					glBindFramebuffer(GL_FRAMEBUFFER, FBO[0]->FBO);
				else
					glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				for (auto it{ renderables.begin() }; it != renderables.end(); ++it)
					(*it)->Draw(this->GetCameraMatrix(), this->GetPerspectiveMatrix());

				for (int i{ 0 }; i < FBO.size(); ++i)
				{
					if (i == FBO.size() - 1)
						glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
					else
						glBindFramebuffer(GL_FRAMEBUFFER, FBO[i + 1]->FBO);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					FBO[i]->data.s->UseProgram();
					glUniform3fv(FBO[i]->data.s->GetLocation("uColor"), 1, m_color.xyz);
					glBindTexture(GL_TEXTURE_2D, FBO[i]->ColorTexture);
					glBindVertexArray(FBO[i]->data.VAO);

					glDrawArrays(GL_TRIANGLES, 0, 6);

					glBindTexture(GL_TEXTURE_2D, 0);
					glBindVertexArray(0);
				}
			}
	}

	void PlayerCamera::OnCopy(IComponent* copyTo) const
	{
		Camera::OnCopy(copyTo);
		PlayerCamera* copy{ dynamic_cast<PlayerCamera*>(copyTo) };

		copy->m_speed = m_speed;
		copy->m_runningSpeed = m_runningSpeed;
		copy->m_minMoveLength = m_minMoveLength;
		copy->m_moveLerpSpeed = m_moveLerpSpeed;
		copy->m_movement = m_movement;
		copy->m_isRotating = m_isRotating;
		copy->m_isMouseSet = m_isMouseSet;
		copy->m_angularSpeed = m_angularSpeed;
		copy->m_pitch = m_pitch;
		copy->m_yaw = m_yaw;
		copy->m_minAngularLength = m_minAngularLength;
		copy->m_angularLerpSpeed = m_angularLerpSpeed;
		copy->m_angularMovement = m_angularMovement;
		copy->m_mousePos = m_mousePos;
	}

	void PlayerCamera::StartCopy(IComponent*& copyTo) const
	{
		PlayerCamera* test = new PlayerCamera();
		copyTo = test;
		OnCopy(copyTo);
	}

	void	PlayerCamera::OnReset()
	{
		Camera::OnReset();

		m_isRunning = false;
		m_speed = 5.f;
		m_runningSpeed = 20.f;
		m_minMoveLength = 0.001f;
		m_moveLerpSpeed = 0.5f;

		m_isRotating = false;
		m_isMouseSet = false;
		m_angularSpeed = 0.1f;
		m_yaw = 0.f;
		m_pitch = 0.f;
		m_minAngularLength = 0.001f;
		m_angularLerpSpeed = 0.5;
	}

	void PlayerCamera::OnDestroy()
	{
		Core::Datastructure::ICamera::OnDestroy();

		for (int i{ 0 }; i < m_postProcessFBO.size(); ++i)
			GetRoot()->GetEngine()->DeleteFBO(m_postProcessFBO[i]);
		m_postProcessFBO.clear();
	}
}
