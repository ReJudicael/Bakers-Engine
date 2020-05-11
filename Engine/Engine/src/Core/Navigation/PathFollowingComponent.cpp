#include "PathFollowingComponent.h"
#include "NavMesh.h"
#include "RootObject.hpp"
#include "EngineCore.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Navigation;
	rttr::registration::class_<PathFollowingComponent>("PathFollowing")
		.constructor()
		.property("Target", &PathFollowingComponent::GetTarget, &PathFollowingComponent::SetTarget)
		.property("Speed", &PathFollowingComponent::m_moveSpeed);
}

namespace Core::Navigation
{
	bool PathFollowingComponent::OnStart()
	{
		Core::Navigation::NavMeshBuilder* nav{ GetRoot()->GetEngine()->GetNavMesh() };
		if (!nav)
			return false;
		FindPath(m_target);
		if (m_pathQuery == nullptr)
			return false;
		return IUpdatable::OnStart();
	}
	void PathFollowingComponent::OnDestroy()
	{
		ComponentBase::OnDestroy();
		INavAgent::OnDestroy();
		IUpdatable::OnDestroy();
	}

	void PathFollowingComponent::UpdatePos()
	{
		if (m_pathIndex >= m_path.straightPathSize)
			return;
		Core::Maths::Vec3 toDest{ (m_path.straightPath[m_pathIndex] - GetParent()->GetGlobalPos()) };
		toDest.y = 0;
		std::cout << toDest.SquaredLength() << std::endl;
		if (toDest.SquaredLength() < m_destPrecision * m_destPrecision)
		{
			++m_pathIndex;
			UpdatePos();
		}
		else
			GetParent()->TranslateGlobal(toDest.Normalized() * m_moveSpeed);
	}

	PathFollowingComponent::PathFollowingComponent() : ComponentBase(), INavAgent()
	{
	}

	void Core::Navigation::PathFollowingComponent::SetTarget(Core::Maths::Vec3 target)
	{
		m_target = target;

		if (!IsStarted())
			return;

		FindPath(target);
	}

	void PathFollowingComponent::OnUpdate(float deltaTime)
	{
		if (m_pathQuery != nullptr && dtStatusSucceed(m_pathQuery->status))
		{
			m_path = m_pathQuery->path;
			delete m_pathQuery;
			m_pathQuery = nullptr;
			m_pathIndex = 0;

			std::cout << "test path" << std::endl;

			for (int i = 0; i < m_path.straightPathSize; ++i)
			{
				m_path.straightPath[i].Print();
			}
		}
		UpdatePos();
	}

	void PathFollowingComponent::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new PathFollowingComponent();
		OnCopy(copyTo);
	}

	void PathFollowingComponent::OnCopy(IComponent* copyTo) const
	{
		INavAgent::OnCopy(copyTo);
		IUpdatable::OnCopy(copyTo);
	}

	void PathFollowingComponent::OnReset()
	{
		INavAgent::OnReset();
		IUpdatable::OnReset();
	}
}