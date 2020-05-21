#include "INavAgent.h"
#include "RootObject.hpp"
#include "EngineCore.h"
#include "NavMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	using namespace Core::Datastructure;
	registration::enumeration<NavAgentFlags>("NavAgentFlags")
		(
			value("None", NavAgentFlags::NONE),
			value("Anticipate turns", NavAgentFlags::ANTICIPATE_TURNS),
			value("Optimize vis", NavAgentFlags::OPTIMIZE_VIS),
			value("Optimize topo", NavAgentFlags::OPTIMIZE_TOPO),
			value("Obstacle avoidance", NavAgentFlags::OBSTACLE_AVOIDANCE),
			value("Separation", NavAgentFlags::SEPARATION)
		);

	registration::class_<INavAgent>("INavAgent")
		.constructor();
		//.property_readonly("agentID", &INavAgent::m_agentID);
}

namespace Core::Datastructure
{
	void INavAgent::FindPath(const Core::Maths::Vec3& end, unsigned short excludedAreaFlags)
	{
		Core::Navigation::NavMeshBuilder* navMesh{ GetRoot()->GetEngine()->GetNavMesh() };
		if (m_pathQuery != nullptr)
		{
			navMesh->RemovePathQuery(m_pathQuery);
			delete m_pathQuery;
		}
		m_pathQuery = navMesh->FindPath(GetParent()->GetGlobalPos(), end, excludedAreaFlags);
	}

	void INavAgent::OnDestroy()
	{
		if (m_pathQuery != nullptr)
		{
			GetRoot()->GetEngine()->GetNavMesh()->RemovePathQuery(m_pathQuery);
			delete m_pathQuery;
		}
	}

	INavAgent::INavAgent() : IComponent()
	{
		
	}

	INavAgent::~INavAgent()
	{
	}
}