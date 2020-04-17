#pragma once

#include "IComponent.hpp"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	enum class NavAgentFlags : unsigned char
	{
		NONE = 0,
		ANTICIPATE_TURNS = 1,
		OPTIMIZE_VIS = 1 << 1,
		OPTIMIZE_TOPO = 1 << 2,
		OBSTACLE_AVOIDANCE = 1 << 3,
		SEPARATION = 1 << 4
	};

	RTTR_DECLARE_FLAGS(NavAgentFlag, NavAgentFlags)
	RTTR_DECLARE_ENUM_FLAGS_OPERATORS(NavAgentFlag)

	class INavAgent : virtual public IComponent
	{
	protected:
		int				m_agentID{ -1 };

		float			m_agentHeight{ 2.0f };
		float			m_agentRadius{ 0.6f };
		float			m_agentAcceleration{ 8.f };
		float			m_agentMaxSpeed{ 3.5f };
		float			m_agentMaxClimb{ 0.9f };
		float			m_agentMaxSlope{ 45.0f };
		unsigned char	m_obstacleAvoidanceQuality{ 3 };
		float			m_separationWeight{ 2.0f };

		NavAgentFlag	m_flags{ NavAgentFlags::NONE };

		void			OnStart() override;
		void			OnDestroy() override;
	public:
		INavAgent();
		~INavAgent();

		REGISTER_CLASS(IComponent)
	};
}