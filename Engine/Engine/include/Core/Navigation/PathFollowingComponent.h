#pragma once

#include "CoreMinimal.h"

#include "ComponentBase.h"
#include "INavAgent.h"
#include "IUpdatable.hpp"
#include "RigidBody.h"

namespace Core::Navigation
{
	/**
	 * Handle movement on nav mesh
	 */
	BAKERS_API_CLASS PathFollowingComponent : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::INavAgent, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		Core::Maths::Vec3	m_target;
		NavPath				m_path;
		int					m_pathIndex{ 0 };
		bool				m_stopMoving{false};
		float				m_moveSpeed{ 0.5f };
		float				m_destPrecision{ 0.2f };
		float				m_agentHeight{ 1.f };
		Physics::RigidBody* m_rigidbody;

	protected:
		virtual bool	OnStart() override;
		virtual void	OnDestroy() override;
		virtual void	OnUpdate(float deltaTime) override;

		virtual void	StartCopy(IComponent*& copyTo) const override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	OnReset() override;

		/**
		 * Navigation handling
		 * @param deltaTime: Time between frames
		 */
		void				UpdatePos(float deltaTime);
	public:
		PathFollowingComponent();

		/**
		 * Target getter
		 * @return Current position the component is trying to reach
		 */
		Core::Maths::Vec3	GetTarget() { return m_target; };

		/**
		 * Target setter
		 * @param target: New target for the component
		 */
		void				SetTarget(Core::Maths::Vec3 target);

		/**
		 * Get the stopMoving value
		 * @return true if stopMoving is at true, 
		 * that means that path folling stop moving
		 */
		bool				IsStopMoving() { return m_stopMoving; }
		/**
		 * Set the stopMoving value
		 * @param stopMoving: the bool for stop or not 
		 * the movement of the parent
		 */
		void				SetStopMoving(const bool stopMoving);

		/**
		 * Get if the PathFollowingComponent have finish his path
		 * @return true he finished his path
		 */
		bool				IsEndOfThePath() { return { m_path.straightPathSize > m_pathIndex }; }

		REGISTER_CLASS(ComponentBase, INavAgent, IUpdatable)
	};
}
