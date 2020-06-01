#pragma once

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "Vec3.hpp"
#include "NavPath.h"

#include <queue>

namespace Core::Navigation
{
	BAKERS_API_CLASS NavQuery
	{
	public:
		struct QueryResult
		{
			dtStatus	status;
			NavPath		path;
		};
		struct Query
		{
			Core::Maths::Vec3	start;
			Core::Maths::Vec3	end;

			dtPolyRef			endRef;

			QueryResult*		result;
			dtQueryFilter		filter;
		};
	protected:
		dtNavMeshQuery*		m_query;
		std::queue<Query>	m_queriesQueue;
		Query				m_current;
		dtStatus			m_status;
		unsigned char		m_straightPathFlags[NavPath::MAX_PATH_POLY];

		bool				InitNextQuery();

		void				InitCurrPath();
	public:
		NavQuery();
		bool	Init(dtNavMesh* navMesh);
		bool	IsInit() { return m_query->getAttachedNavMesh() != nullptr; };
		~NavQuery();

		void	AddQuery(const Core::Maths::Vec3& start, const Core::Maths::Vec3& end, const dtQueryFilter& filter, QueryResult* result);
		void	RemoveQuery(QueryResult* toRemove);

		dtStatus	FindNearestPoly(const Core::Maths::Vec3& point, const Core::Maths::Vec3& dist, const dtQueryFilter& filter, dtPolyRef* ref, Core::Maths::Vec3& pos);

		void	Update(int maxIters = 128);
	};
}