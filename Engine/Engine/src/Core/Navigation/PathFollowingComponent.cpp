#include "PathFollowingComponent.h"
#include "NavMesh.h"
#include "RootObject.hpp"
#include "EngineCore.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Navigation;
	rttr::registration::class_<PathFollowingComponent>("Path Following")
		.constructor()
		.property("Target", &PathFollowingComponent::GetTarget, &PathFollowingComponent::SetTarget)
		.property("AgentHeight", &PathFollowingComponent::m_agentHeight)
		.property("Speed", &PathFollowingComponent::m_moveSpeed);
}

namespace Core::Navigation
{
	bool PathFollowingComponent::OnStart()
	{
		ZoneScoped
		Core::Navigation::NavMeshBuilder* nav{ GetRoot()->GetEngine()->GetNavMesh() };
		if (!nav)
			return false;
		FindPath(m_target);
		if (m_pathQuery == nullptr)
			return false;

		std::list<Core::Physics::RigidBody*> rigidBodies;
		m_parent->GetComponentsOfTypeInObject<Core::Physics::RigidBody>(rigidBodies);
		if (rigidBodies.size() > 0)
			m_rigidbody = *rigidBodies.begin();

		return IUpdatable::OnStart();
	}

	void PathFollowingComponent::OnDestroy()
	{
		ComponentBase::OnDestroy();
		INavAgent::OnDestroy();
		IUpdatable::OnDestroy();
	}

	void PathFollowingComponent::UpdatePos(float deltaTime)
	{
		if (m_pathIndex >= m_path.straightPathSize)
			return;

		if (m_rigidbody)
			m_rigidbody->SetLinearVelocity({ 0.f, 0.f, 0.f });

		Core::Maths::Vec3 toDest{ ((m_path.straightPath[m_pathIndex] + Core::Maths::Vec3(0, m_agentHeight, 0)) - GetParent()->GetGlobalPos()) };
		if (toDest.SquaredLength() - m_moveSpeed * m_moveSpeed * deltaTime  * deltaTime < m_destPrecision * m_destPrecision)
		{
			++m_pathIndex;
			UpdatePos(deltaTime);
		}
		else
			GetParent()->TranslateGlobal(toDest.Normalized() * m_moveSpeed * deltaTime);

		toDest.y = 0;
		toDest.Normalize();

		Core::Maths::Vec3 right{ toDest.Cross({0, 1, 0}).Normalized() * -1};
		Core::Maths::Vec3 up{ toDest.Cross(right).Normalized() };

		Core::Maths::Mat4 mat;

		mat.Set<0, 0>(right.x);
		mat.Set<0, 1>(up.x);
		mat.Set<0, 2>(toDest.x);

		mat.Set<1, 0>(right.y);
		mat.Set<1, 1>(up.y);
		mat.Set<1, 2>(toDest.y);

		mat.Set<2, 0>(right.z);
		mat.Set<2, 1>(up.z);
		mat.Set<2, 2>(toDest.z);

		GetParent()->SetRot(mat);


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
			m_pathIndex = 1;
		}
		if(!m_stopMoving)
			UpdatePos(deltaTime);
	}

	void PathFollowingComponent::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
		copyTo = new PathFollowingComponent();
		OnCopy(copyTo);
	}

	void PathFollowingComponent::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
		INavAgent::OnCopy(copyTo);
		IUpdatable::OnCopy(copyTo);
	}

	void PathFollowingComponent::OnReset()
	{
		INavAgent::OnReset();
		IUpdatable::OnReset();
	}

	void PathFollowingComponent::SetStopMoving(const bool stopMoving)
	{
		if (m_stopMoving != stopMoving)
			m_stopMoving = stopMoving;
	}
}