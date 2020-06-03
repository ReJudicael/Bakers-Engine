#pragma once

#include "IComponent.hpp"
#include "NavQuery.h"
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

		Core::Navigation::NavQuery::QueryResult* m_pathQuery{ nullptr };

		void			FindPath(const Core::Maths::Vec3& end, unsigned short excludedAreaFlags = 0);

		virtual void			OnDestroy() override;
	public:
		INavAgent();
		~INavAgent();

		REGISTER_CLASS(IComponent)
	};
}