#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "Vec3.hpp"
#include "BoxCollider.h"
#include "ParticleSystem.h"
#include "AudioSource.h"

BAKERS_GAME_CLASS Warp : public Core::Datastructure::ComponentUpdatable
{
private:
	bool m_canTeleport{ false };
	float m_countdown{ 0.f };
	float m_currentTime{ 0.f };
	Core::Maths::Vec3 m_destination;

	Core::Audio::AudioSource* m_audioSource{ nullptr };
	Core::Renderer::ParticleSystem* m_particles{ nullptr };
	Core::Physics::BoxCollider* m_collider{ nullptr };
	Core::Datastructure::Object* m_objectToMove{ nullptr };

protected:
	/**
	 * Init event
	 */
	virtual void OnInit() override;

	/**
	 * Copies the data of the component into the given component.
	 * Should always be safe to cast pointer to current component type.
	 */
	virtual void OnCopy(IComponent* copyTo) const override;

	/**
	 * Copies the component in the given pointer.
	 * On override, should not call other versions of the function.
	 */
	virtual void StartCopy(IComponent*& copyTo) const override;

	/**
	 * First frame upon creation event
	 */
	virtual bool OnStart() override;

	/**
	 * Destroy event
	 */
	virtual void OnDestroy() override;

	/**
	 * Reset event
	 */
	virtual void OnReset() override;

	/**
	 * Call Update function
	 * @param deltaTime: Time elapsed between two frames
	 */
	virtual void OnUpdate(float deltaTime) override;

public:
	/**
	 * Default Constructor
	 */
	Warp();

	/**
	 * Destructor
	 */
	~Warp();

private:
	/**
	 * The parent can use the warp
	 * @param collider: Collider for trigger enter
	 */
	void Activate(Core::Physics::Collider* collider);

	/**
	 * The parent can't use the warp
	 * @param collider: Collider for trigger exit
	 */
	void Deactivate(Core::Physics::Collider* collider);

	REGISTER_CLASS(ComponentUpdatable)
};

