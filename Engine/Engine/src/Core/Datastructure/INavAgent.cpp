#include "INavAgent.h"
#include "RootObject.hpp"
#include "EngineCore.h"

RTTR_PLUGIN_REGISTRATION
{
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
		.constructor()
		.property_readonly("agentID", &INavAgent::m_agentID);
}

namespace Core::Datastructure
{
	INavAgent::INavAgent() : IComponent()
	{
		
	}

	bool INavAgent::OnStart()
	{
		//Navigation::NavMeshBuilder* navMesh{ GetScene()->GetEngine()->GetNavMesh() };
		return true;
	}

	void	INavAgent::OnDestroy()
	{
		
	}

	INavAgent::~INavAgent()
	{
	}
}