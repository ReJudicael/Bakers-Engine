#pragma once

#include "CoreMinimal.h"

#include "Transform.hpp"

#include "Recast.h"
#include "DetourNavMesh.h"
#include <chrono>


namespace Core::Navigation
{
	struct NavMesh
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
		unsigned long		m_maxTris{ 0 };
		bool				m_isUpdated{ false };

		rcConfig			m_cfg;
		BuildContext* m_ctx;
		rcPolyMesh* m_pmesh;
		rcPolyMeshDetail* m_dmesh;
		dtNavMesh* m_navMesh;
		dtNavMeshQuery* m_navQuery;

		std::list<NavMesh>	m_mesh;
	public:
		NavMeshBuilder();
		~NavMeshBuilder();

		bool		Build();
		NavMesh* AddMesh(float* verts, int nverts, int* tris, int ntris, const Core::Datastructure::Transform& position);

		bool		IsNavmeshUpdated() const { return m_isUpdated; };
	};
}
