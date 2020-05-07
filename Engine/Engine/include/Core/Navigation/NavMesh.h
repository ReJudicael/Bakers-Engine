#pragma once

#include "CoreMinimal.h"

#include "Transform.hpp"

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "NavQuery.h"
//#include "DetourCrowd.h"
#include <chrono>
#include <queue>

#define MAX_AGENTS 256

namespace Core::Navigation
{
	struct Mesh
	{
		float*		verts;
		int*		tris;
		int			nverts;
		int			ntris;
	};

	class BuildContext : public rcContext
	{
		std::chrono::high_resolution_clock::time_point m_startTime[RC_MAX_TIMERS];
		std::chrono::high_resolution_clock::duration m_accTime[RC_MAX_TIMERS];

	protected:
		virtual void doResetLog();

		virtual void doLog(const rcLogCategory category, const char* msg, const int len);

		virtual void doResetTimers();

		virtual void doStartTimer(const rcTimerLabel label);

		virtual void doStopTimer(const rcTimerLabel label);

		virtual int doGetAccumulatedTime(const rcTimerLabel label) const;
	};

	BAKERS_API_CLASS NavMeshBuilder
	{
	protected:
		size_t		m_maxTris{ 0 };
		bool				m_isUpdated{ false };

		rcConfig			m_cfg;
		BuildContext*		m_ctx;
		rcPolyMesh*			m_pmesh;
		rcPolyMeshDetail*	m_dmesh;
		dtNavMesh*			m_navMesh;
		NavQuery			m_navQuery;
		dtQueryFilter		m_queryFilter;
		//dtCrowd*			m_crowd;

		std::list<Mesh>	m_mesh;
		/*
		dtStatus		FindNearestPoly(const Core::Maths::Vec3& targetPos, const dtQueryFilter* filter, dtPolyRef* outRef, Core::Maths::Vec3& outPoint) const noexcept;
		dtStatus		FindPath(dtPolyRef startRef, dtPolyRef endRef, const Core::Maths::Vec3& startPos, const Core::Maths::Vec3& endPos, const dtQueryFilter* filter, NavPath& path) const noexcept;
		*/
	public:
		NavMeshBuilder();
		NavMeshBuilder(const NavMeshBuilder&) = delete;
		NavMeshBuilder(NavMeshBuilder&&) = delete;
		~NavMeshBuilder();

		bool			Build();
		Mesh*			AddMesh(float* verts, int nverts, int* tris, int ntris, const Core::Datastructure::Transform& position);

		bool			IsNavmeshUpdated() const { return m_isUpdated; }

		//dtCrowd*		GetCrowd() const noexcept { return m_crowd; }

		NavQuery::QueryResult*	FindPath(const Core::Maths::Vec3& start, const Core::Maths::Vec3& end, unsigned short excludedAreaFlags = 0) noexcept;
		void					RemovePathQuery(NavQuery::QueryResult* toRemove);
		void					UpdateQuery();
		const rcConfig& GetConfig() { return m_cfg; };

		/**
		 * Allocates new nav querry
		 */
		//NavQuery*	GetNavQuery() const noexcept;
	};
}
