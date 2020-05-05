#include "NavQuery.h"
#include "Vec3.hpp"

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

namespace Core::Navigation
{
	bool NavQuery::InitNextQuery()
	{
		if (m_queriesQueue.size() == 0)
			return false;
		m_current = m_queriesQueue.front();
		m_queriesQueue.pop();
		dtPolyRef	startRef, endRef;
		float		startPos[3], endPos[3];

		constexpr float	halfExtent[3]{ 100.f, 100.f, 100.f };

		m_query->findNearestPoly(m_current.start.xyz, halfExtent, &m_current.filter, &startRef, startPos);
		m_query->findNearestPoly(m_current.end.xyz, halfExtent, &m_current.filter, &endRef, endPos);
		m_query->initSlicedFindPath(startRef, endRef, startPos, endPos, &m_current.filter);
		m_current.endRef = endRef;

		m_status = DT_IN_PROGRESS;

		return true;
	}

	void NavQuery::InitCurrPath()
	{
		Core::Maths::Vec3	endPos{ m_current.end };
		if (m_current.result->path.polyRefPath[(size_t)m_current.result->path.numPolys - 1] != m_current.endRef)
			m_query->closestPointOnPoly(m_current.result->path.polyRefPath[(size_t)m_current.result->path.numPolys - 1], m_current.end.xyz, endPos.xyz, 0);
		
		float	straightPath[NavPath::MAX_PATH_POLY];
		m_query->findStraightPath(m_current.start.xyz, endPos.xyz, m_current.result->path.polyRefPath.data(), 
			m_current.result->path.numPolys, straightPath, m_straightPathFlags, m_current.result->path.polyRefStraightPath.data(), 
			&m_current.result->path.straightPathSize, static_cast<int>(m_current.result->path.polyRefPath.size()), DT_STRAIGHTPATH_ALL_CROSSINGS);
		for (int i = 0; i < NavPath::MAX_PATH_POLY; ++i)
			m_current.result->path.straightPath[i] = Core::Maths::Vec3(straightPath[i * 3], straightPath[i * 3 + 1], straightPath[i * 3 + 2]);
	}

	NavQuery::NavQuery() : m_straightPathFlags()
	{
		m_query = dtAllocNavMeshQuery();
		m_status = DT_FAILURE;
	}

	bool NavQuery::Init(dtNavMesh* navMesh)
	{
		return dtStatusSucceed(m_query->init(navMesh, 2048));
	}

	NavQuery::~NavQuery()
	{
		dtFreeNavMeshQuery(m_query);
	}

	void NavQuery::AddQuery(const Core::Maths::Vec3& start, const Core::Maths::Vec3& end, const dtQueryFilter& filter, QueryResult* result)
	{
		if (result == nullptr)
			return;
		m_queriesQueue.push({ start, end, dtPolyRef(-1), result, filter });
		result->status = DT_IN_PROGRESS;
	}

	void NavQuery::Update(int maxIters)
	{
		if (!dtStatusInProgress(m_status))
		{
			if (!InitNextQuery())
				return;
		}
		int iter;
		dtStatus status = m_query->updateSlicedFindPath(maxIters, &iter);
		if (dtStatusSucceed(status))
		{
			int max;
			m_current.result->status = m_query->finalizeSlicedFindPath(m_current.result->path.polyRefPath.data(), &max, static_cast<int>(m_current.result->path.polyRefPath.size()));
			m_current.result->path.numPolys = max;
			if (max)
				InitCurrPath();
			m_status = DT_SUCCESS;
			Update(maxIters - iter);
		}
	}
}