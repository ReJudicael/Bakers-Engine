#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Vec3.hpp"
#include "Brioche.h"
#include "PhysicsScene.h"
#include "Camera.h"
#include "Owen.h"
#include "Light.h"

// Raycast target to move ally
BAKERS_GAME_CLASS Target : public Core::Datastructure::ComponentUpdatable
{
	Brioche*	m_follower;

	Core::Physics::PhysicsScene* m_physicsScenePtr{ nullptr };
	Core::Renderer::Camera* m_playerCamera{ nullptr };
	Core::Renderer::Light* m_signal{ nullptr };

	bool	m_isLeading{ false };

	protected:
		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;
		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * First frame upon creation event
		 */
		virtual bool	OnStart() override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;

		/**
		 * Reset event
		 */
		virtual void	OnReset() override;

		/**
		 * Search for player camera in scene and set the value of m_playerCamera
		 */
		void	FindPlayerCamera();

		/**
		 * Set Parent global position depending on inputs
		 */
		void	SetTargetPosition();

public:
	/**
	 * Default Constructor
	 */
	Target();

	/**
	 * Destructor
	 */
	~Target();

	/**
	 * Init event
	 */
	void	OnInit() override;

	/**
	 * Update event
	 * @param deltaTime: Time elapsed between two frames
	 */
	 virtual void OnUpdate(float deltaTime) override;

	 REGISTER_CLASS(ComponentUpdatable)
};




