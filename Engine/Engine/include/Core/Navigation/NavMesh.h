#pragma once

#include "CoreMinimal.h"

#include "Transform.hpp"

#include "Recast.h"
#include "DetourNavMesh.h"

namespace Core::Navigation
{
	struct NavMesh
	{
		float*		verts;
		int*		tris;
		int			nverts;
		int			ntris;
	};

	class NavMeshBuilder
	{
	protected:
		std::list<NavMesh>	m_mesh;

		rcConfig			m_cfg;
		rcContext*			m_ctx;

		unsigned long		m_maxTris{ 0 };

		rcPolyMesh*			m_pmesh;
		rcPolyMeshDetail*	m_dmesh;
		dtNavMesh*			m_navMesh;
		dtNavMeshQuery*		m_navQuery;
	public:
		NavMeshBuilder();
		~NavMeshBuilder();

		bool		Build();
		NavMesh*	AddMesh(float* verts, int nverts, int* tris, int ntris, const Core::Datastructure::Transform& position);
	};
}
