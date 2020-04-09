#include "NavMesh.h"

#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"

namespace Core::Navigation
{
	NavMeshBuilder::NavMeshBuilder()
	{
		m_cfg.cs = 0.3f;
		m_cfg.ch = 0.2f;
		m_cfg.walkableSlopeAngle = 45.f;
		m_cfg.walkableHeight = (int)ceilf(2.0f / m_cfg.ch);
		m_cfg.walkableClimb = (int)floorf(0.9f / m_cfg.ch);
		m_cfg.walkableRadius = (int)ceilf(0.6f / m_cfg.cs);
		m_cfg.maxEdgeLen = (int)(12.0f / m_cfg.cs);
		m_cfg.maxSimplificationError = 1.3f;
		m_cfg.minRegionArea = (int)rcSqr(8);		// Note: area = size*size
		m_cfg.mergeRegionArea = (int)rcSqr(20);	// Note: area = size*size
		m_cfg.maxVertsPerPoly = (int)6.0f;
		m_cfg.detailSampleDist = 6.0f < 0.9f ? 0 : m_cfg.cs * 6.0f;
		m_cfg.detailSampleMaxError = m_cfg.ch * 1.0f;

		m_cfg.bmin[0] = -1.f;
		m_cfg.bmin[1] = -1.f;
		m_cfg.bmin[2] = -1.f;
		m_cfg.bmax[0] = 1.f;
		m_cfg.bmax[1] = 1.f;
		m_cfg.bmax[2] = 1.f;

		m_navQuery = dtAllocNavMeshQuery();
	}
	NavMeshBuilder::~NavMeshBuilder()
	{
		for (auto it{ m_mesh.begin() }; it != m_mesh.end(); ++it)
		{
			delete it->verts;
			delete it->tris;
		}

		rcFreePolyMesh(m_pmesh);
	}

	bool NavMeshBuilder::Build()
	{
		rcFreePolyMesh(m_pmesh);
		rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

		rcHeightfield* solid{ rcAllocHeightfield() };
		if (!solid)
		{
			DEBUG_LOG_ERROR("Failed to allocate solid heightfield");
			return false;
		}
		if (!rcCreateHeightfield(m_ctx, *solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
		{
			DEBUG_LOG_ERROR("Failed to create solid heightfield");
			return false;
		}

		unsigned char* triareas{ new unsigned char[m_maxTris] };
		if (!triareas)
		{
			DEBUG_LOG_ERROR(std::string("Failed to allocate ") + std::to_string(m_maxTris) + std::string(" bytes for triarea"));
			return false;
		}
		for (auto it{ m_mesh.begin() }; it != m_mesh.end(); ++it)
		{
			if (it->ntris > m_maxTris)
			{
				m_maxTris = it->ntris;
				delete[] triareas;
				triareas = new unsigned char[m_maxTris];
			}
			memset(triareas, 0, m_maxTris * sizeof(unsigned char));
			rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, it->verts, it->nverts, it->tris, it->ntris, triareas);
			if (!rcRasterizeTriangles(m_ctx, it->verts, it->nverts, it->tris, triareas, it->ntris, *solid, m_cfg.walkableClimb))
			{
				DEBUG_LOG_ERROR("Could not rasterize triangles");
				return false;
			}
		}
		delete[] triareas;

		//Maybe add checks for filters later
		//LowHangingObstacles
		rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *solid);
		//LedgeSpans
		rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *solid);
		//WalkableLowHeightSpans
		rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *solid);

		rcCompactHeightfield* chf{ rcAllocCompactHeightfield() };
		if (!chf)
		{
			DEBUG_LOG_ERROR("Failed to allocate compact height field");
			return false;
		}
		if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *solid, *chf))
		{
			DEBUG_LOG_ERROR("Could not build compact height field data");
			return false;
		}
		rcFreeHeightField(solid);

		if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *chf))
		{
			DEBUG_LOG_ERROR("Failed to erode navigation data");
			return false;
		}

		//Watershed partitioning
		if (!rcBuildDistanceField(m_ctx, *chf))
		{
			DEBUG_LOG_ERROR("Could not build distance field");
			return false;
		}
		if (!rcBuildRegions(m_ctx, *chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			DEBUG_LOG_ERROR("Could not build watershed regions");
			return false;
		}

		rcContourSet* cset = rcAllocContourSet();

		if (!cset)
		{
			DEBUG_LOG_ERROR("Failed to allocate contour set");
			return false;
		}
		if (!rcBuildContours(m_ctx, *chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *cset))
		{
			DEBUG_LOG_ERROR("Could not create contours");
			return false;
		}

		m_pmesh = rcAllocPolyMesh();
		if (!m_pmesh)
		{
			DEBUG_LOG_ERROR("Failed to allocate poly mesh");
			return false;
		}
		if (!rcBuildPolyMesh(m_ctx, *cset, m_cfg.maxVertsPerPoly, *m_pmesh))
		{
			DEBUG_LOG_ERROR("Could not triangulate contours");
			return false;
		}

		m_dmesh = rcAllocPolyMeshDetail();
		if (!m_dmesh)
		{
			DEBUG_LOG_ERROR("Failed to allocate poly mesh detail");
			return false;
		}

		if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
		{
			DEBUG_LOG_ERROR("Could not build detail mesh");
			return false;
		}
		rcFreeCompactHeightfield(chf);
		rcFreeContourSet(cset);

		if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
		{
			unsigned char* navData = 0;
			int navDataSize = 0;

			// Update poly flags from areas.
			for (int i = 0; i < m_pmesh->npolys; ++i)
			{
				if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
					m_pmesh->flags[i] = 1;
			}


			dtNavMeshCreateParams params;
			memset(&params, 0, sizeof(params));
			params.verts = m_pmesh->verts;
			params.vertCount = m_pmesh->nverts;
			params.polys = m_pmesh->polys;
			params.polyAreas = m_pmesh->areas;
			params.polyFlags = m_pmesh->flags;
			params.polyCount = m_pmesh->npolys;
			params.nvp = m_pmesh->nvp;
			params.detailMeshes = m_dmesh->meshes;
			params.detailVerts = m_dmesh->verts;
			params.detailVertsCount = m_dmesh->nverts;
			params.detailTris = m_dmesh->tris;
			params.detailTriCount = m_dmesh->ntris;
			/*
			params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
			params.offMeshConRad = m_geom->get\OffMeshConnectionRads();
			params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
			params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
			params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
			params.offMeshConUserID = m_geom->getOffMeshConnectionId();
			*/
			params.offMeshConCount = 0;
			params.walkableHeight = m_cfg.walkableHeight * m_cfg.ch;
			params.walkableRadius = m_cfg.walkableRadius * m_cfg.cs;
			params.walkableClimb = m_cfg.walkableClimb * m_cfg.ch;
			rcVcopy(params.bmin, m_pmesh->bmin);
			rcVcopy(params.bmax, m_pmesh->bmax);
			params.cs = m_cfg.cs;
			params.ch = m_cfg.ch;
			params.buildBvTree = true;

			if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
			{
				DEBUG_LOG_ERROR("Could not build Detour navmesh");
				return false;
			}

			m_navMesh = dtAllocNavMesh();
			if (!m_navMesh)
			{
				dtFree(navData);
				DEBUG_LOG_ERROR("Could not create Detour navmesh");
				return false;
			}

			dtStatus status;

			status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
			if (dtStatusFailed(status))
			{
				dtFree(navData);
				DEBUG_LOG_ERROR("Could not init Detour navmesh");
				return false;
			}

			status = m_navQuery->init(m_navMesh, 2048);
			if (dtStatusFailed(status))
			{
				DEBUG_LOG_ERROR("Could not init Detour navmesh query");
				return false;
			}
		}

		return true;
	}

	NavMesh* NavMeshBuilder::AddMesh(float* verts, int nverts, int* tris, int ntris, const Core::Datastructure::Transform& position)
	{
		float* newVerts{ (float*)malloc(nverts * sizeof(float*)) };
		Core::Maths::Quat	rot{ position.GetGlobalRot() };
		Core::Maths::Quat	rotInv{ rot.Inversed() };
		Core::Maths::Vec3	pos{ position.GetGlobalPos() };

		Core::Maths::Vec3 vert;

		for (unsigned i{ 0 }; i < nverts / 3; ++i)
		{
			vert = { verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2] };
			vert = pos + (rot * Core::Maths::Quat(0, vert) * rotInv).GetVec();
			newVerts[i * 3] = vert.x;
			newVerts[i * 3 + 1] = vert.y;
			newVerts[i * 3 + 2] = vert.z;
		}

		int* newTris{ (int*)malloc(ntris * sizeof(int*)) };
		for (unsigned i{ 0 }; i < ntris; ++i)
		{
			newTris[i] = tris[i];
		}

		m_mesh.push_back({ newVerts, newTris, nverts, ntris });

		if (m_maxTris < ntris)
			m_maxTris = ntris;

		return &m_mesh.back();
	}
}
