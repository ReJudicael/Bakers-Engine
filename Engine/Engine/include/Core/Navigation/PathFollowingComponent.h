#pragma once

#include "CoreMinimal.h"

#include "ComponentBase.h"
#include "INavAgent.h"
#include "IUpdatable.hpp"
#include "RigidBody.h"

namespace Core::Navigation
{
	BAKERS_API_CLASS PathFollowingComponent : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::INavAgent, public virtual Core::Datastructure::IUpdatable
	{
	protected:
		virtual bool	OnStart() override;
		virtual void	OnDestroy() override;
		virtual void	OnUpdate(float deltaTime) override;

		virtual void	StartCopy(IComponent*& copyTo) const override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	OnReset() override;

		Core::Maths::Vec3	m_target;
		NavPath				m_path;
		int					m_pathIndex{ 0 };

		float				m_moveSpeed{ 0.5f };
		float				m_destPrecision{ 0.2f };
		float				m_agentHeight{ 1.f };

		Physics::RigidBody* m_rigidbody;

		void				UpdatePos();
	public:
		PathFollowingComponent();

		Core::Maths::Vec3	GetTarget() { return m_target; };
		void				SetTarget(Core::Maths::Vec3 target);

		REGISTER_CLASS(ComponentBase, INavAgent, IUpdatable)
	};
}
