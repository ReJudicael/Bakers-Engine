#include "NavMesh.h"

#include "DetourNavMeshBuilder.h"
#include "IRenderable.hpp"
#include "EngineCore.h"
#include "Shader.h"

namespace Core::Navigation
{
	NavMeshBuilder::NavMeshBuilder(Core::Datastructure::EngineCore* engine) : m_engine{ engine }
	{
		ZoneScoped
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

		m_cfg.bmin[0] = -100.f;
		m_cfg.bmin[1] = -100.f;
		m_cfg.bmin[2] = -100.f;
		m_cfg.bmax[0] = 100.f;
		m_cfg.bmax[1] = 100.f;
		m_cfg.bmax[2] = 100.f;

		m_ctx = new BuildContext();
	}

	NavMeshBuilder::~NavMeshBuilder()
	{
		ZoneScoped
		for (auto it{ m_mesh.begin() }; it != m_mesh.end(); ++it)
		{
			delete it->verts;
			delete it->tris;
		}

		rcFreePolyMesh(m_pmesh);

		dtFreeNavMesh(m_navMesh);
		delete m_ctx;
	}

	bool NavMeshBuilder::Build()
	{
		ZoneScoped
		if (m_mesh.size() == 0 || m_maxTris == 0)
		{

			rcFreePolyMesh(m_pmesh);
			m_pmesh = nullptr;
			dtFreeNavMesh(m_navMesh);
			m_navMesh = nullptr;
			return false;
		}

		m_isUpdated = true;

		rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

		auto chrono{ std::chrono::high_resolution_clock::now() };

		rcHeightfield* solid{ rcAllocHeightfield() };
		if (!solid)
		{
			BAKERS_LOG_ERROR("Failed to allocate solid heightfield");
			return false;
		}
		if (!rcCreateHeightfield(m_ctx, *solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
		{
			BAKERS_LOG_ERROR("Failed to create solid heightfield");
			return false;
		}

		unsigned char* triareas{ new unsigned char[m_maxTris] };
		if (!triareas)
		{
			BAKERS_LOG_ERROR(std::string("Failed to allocate ") + std::to_string(m_maxTris) + std::string(" bytes for triarea"));
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
				BAKERS_LOG_ERROR("Could not rasterize triangles");
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
			BAKERS_LOG_ERROR("Failed to allocate compact height field");
			return false;
		}
		if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *solid, *chf))
		{
			BAKERS_LOG_ERROR("Could not build compact height field data");
			return false;
		}
		rcFreeHeightField(solid);

		if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *chf))
		{
			BAKERS_LOG_ERROR("Failed to erode navigation data");
			return false;
		}

		//Watershed partitioning
		if (!rcBuildDistanceField(m_ctx, *chf))
		{
			BAKERS_LOG_ERROR("Could not build distance field");
			return false;
		}
		if (!rcBuildRegions(m_ctx, *chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			BAKERS_LOG_ERROR("Could not build watershed regions");
			return false;
		}

		rcContourSet* cset = rcAllocContourSet();

		if (!cset)
		{
			BAKERS_LOG_ERROR("Failed to allocate contour set");
			return false;
		}
		if (!rcBuildContours(m_ctx, *chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *cset))
		{
			BAKERS_LOG_ERROR("Could not create contours");
			return false;
		}

		rcFreePolyMesh(m_pmesh);
		m_pmesh = rcAllocPolyMesh();
		if (!m_pmesh)
		{
			BAKERS_LOG_ERROR("Failed to allocate poly mesh");
			return false;
		}
		if (!rcBuildPolyMesh(m_ctx, *cset, m_cfg.maxVertsPerPoly, *m_pmesh))
		{
			BAKERS_LOG_ERROR("Could not triangulate contours");
			return false;
		}

		m_dmesh = rcAllocPolyMeshDetail();
		if (!m_dmesh)
		{
			BAKERS_LOG_ERROR("Failed to allocate poly mesh detail");
			return false;
		}

		if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
		{
			BAKERS_LOG_ERROR("Could not build detail mesh");
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
				BAKERS_LOG_ERROR("Could not build Detour navmesh");
				return false;
			}

			m_navMesh = dtAllocNavMesh();
			if (!m_navMesh)
			{
				dtFree(navData);
				BAKERS_LOG_ERROR("Could not create Detour navmesh");
				return false;
			}

			dtStatus status;

			status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
			if (dtStatusFailed(status))
			{
				dtFree(navData);
				BAKERS_LOG_ERROR("Could not init Detour navmesh");
				return false;
			}
			
			if (!m_navQuery.Init(m_navMesh))
			{
				BAKERS_LOG_ERROR("Could not init Detour navmesh query");
				return false;
			}
		}

		const std::chrono::high_resolution_clock::time_point end{ std::chrono::high_resolution_clock::now() };
		auto delta{ end - chrono };
		std::chrono::steady_clock::duration time = delta;

		long long ns{ time.count() % 1000ll };
		long long us{ (time.count() / 1000ll) % 1000ll };
		long long ms{ (time.count() / 1000000ll) % 1000ll };
		long long s{ (time.count() / 1000000000ll) % 1000ll };
		std::string timeStr{ "Total build time for Navmesh: " };
		bool comma{ false }; //Put comma before text
		if (s > 0)
		{
			if (comma)
				timeStr += ", ";
			timeStr += std::to_string(s) + " seconds";
			comma = true;
		}
		if (ms > 0) 
		{
			if (comma)
				timeStr += ", ";
			timeStr += std::to_string(ms) + " miliseconds";
			comma = true;
		}
		if (us > 0)
		{
			if (comma)
				timeStr += ", ";
			timeStr += std::to_string(us) + " microseconds";
			comma = true;
		}
		if (ns > 0)
		{
			if (comma)
				timeStr += " and ";
			timeStr += std::to_string(ns) + " nanoseconds";
			comma = true;
		}

		BAKERS_LOG_MESSAGE(timeStr);

		BuildNavMeshRenderer();
		
		return true;
	}

	Mesh* NavMeshBuilder::AddMesh(float* verts, int nverts, int* tris, int ntris, const Core::Datastructure::Transform& position)
	{
		ZoneScoped
		float* newVerts{ new float[nverts] };

		Core::Maths::Quat	rot{ position.GetGlobalRot() };
		Core::Maths::Quat	rotInv{ rot.Inversed() };
		Core::Maths::Vec3	pos{ position.GetGlobalPos() };
		Core::Maths::Vec3	scale{ position.GetGlobalScale() };

		Core::Maths::Vec3 vert;

		for (int i{ 0 }; i < nverts / 3; ++i)
		{
			vert = { verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2] };
			vert = pos + (rot * Core::Maths::Quat(0, vert * scale) * rotInv).GetVec();
			newVerts[i * 3] = vert.x;
			newVerts[i * 3 + 1] = vert.y;
			newVerts[i * 3 + 2] = vert.z;
		}

		int* newTris{ new int[ntris] };
		int max = 0;
		for (int i{ 0 }; i < ntris; ++i)
		{
			if (tris[i] > max)
				max = tris[i];
			newTris[i] = tris[i];
		}

		m_isUpdated = false;
		m_mesh.push_back({ newVerts, newTris, nverts, ntris / 3 });

		if (m_maxTris < ntris)
			m_maxTris = ntris;

		return &m_mesh.back();
	}

	void NavMeshBuilder::ClearInputMeshes()
	{
		ZoneScoped
		for (auto it : m_mesh)
		{
			delete it.tris;
			delete it.verts;
		}
		m_mesh.clear();
		m_maxTris = 0;
	}

	void BuildContext::doResetLog()
	{
	}

	void BuildContext::doLog(const rcLogCategory type, const char* message, const int length)
	{
		Core::Debug::ELogType logType;
		switch (type)
		{
		default:
		case RC_LOG_PROGRESS:
			logType = Core::Debug::ELogType::LOG_MESSAGE;
			break;

		case RC_LOG_WARNING:
			logType = Core::Debug::ELogType::LOG_WARNING;
			break;

		case RC_LOG_ERROR:
			logType = Core::Debug::ELogType::LOG_ERROR;
			break;
		}
		std::string s{ message, static_cast<size_t>(length) };

		Core::Debug::Logger::AddLog(logType, s, "NavMeshBuilder::NavMesh.cpp", "Build", 0);
	}

	NavQuery::QueryResult* NavMeshBuilder::FindPath(const Core::Maths::Vec3& start, const Core::Maths::Vec3& end, unsigned short excludedAreaFlags) noexcept
	{
		ZoneScoped
		if (!m_navQuery.IsInit())
			return nullptr;

		NavQuery::QueryResult* query = new NavQuery::QueryResult();
		m_navQuery.AddQuery(start, end, m_queryFilter, query);
		m_navQuery.Update(10);

		return query;
	}

	dtStatus NavMeshBuilder::FindClosestPointOnNavMesh(const Core::Maths::Vec3& point, const Core::Maths::Vec3& dist, dtPolyRef* ref, Core::Maths::Vec3& pos)
	{
		ZoneScoped
		if (!m_navQuery.IsInit())
			return DT_FAILURE;
		return m_navQuery.FindNearestPoly(point, dist, m_queryFilter, ref, pos);
	}

	void NavMeshBuilder::RemovePathQuery(NavQuery::QueryResult* toRemove)
	{
		ZoneScoped
		m_navQuery.RemoveQuery(toRemove);
	}

	void NavMeshBuilder::UpdateQuery()
	{
		ZoneScoped
		m_navQuery.Update();
	}

	inline int bit(int a, int b)
	{
		return (a & (1 << b)) >> b;
	}

	inline unsigned int duRGBA(int r, int g, int b, int a)
	{
		return ((unsigned int)r) | ((unsigned int)g << 8) | ((unsigned int)b << 16) | ((unsigned int)a << 24);
	}

	unsigned int duIntToCol(int i, int a)
	{
		int	r = bit(i, 1) + bit(i, 3) * 2 + 1;
		int	g = bit(i, 2) + bit(i, 4) * 2 + 1;
		int	b = bit(i, 0) + bit(i, 5) * 2 + 1;
		return duRGBA(r * 63, g * 63, b * 63, a);
	}

	void NavMeshBuilder::BuildNavMeshRenderer()
	{
		ZoneScoped
		std::vector<NavVertex>	vertex;
		const dtNavMesh* mesh{ m_navMesh };
		for (int l = 0; l < mesh->getMaxTiles(); ++l)
		{
			const dtMeshTile* tile = mesh->getTile(l);
			if (!tile->header) continue;

			dtPolyRef base = mesh->getPolyRefBase(tile);

			int tileNum = mesh->decodePolyIdTile(base);
			const unsigned int tileColor = duIntToCol(tileNum, 128);

			for (int i = 0; i < tile->header->polyCount; ++i)
			{
				const dtPoly* p = &tile->polys[i];
				if (p->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip off-mesh links.
					continue;

				const dtPolyDetail* pd = &tile->detailMeshes[i];
				unsigned int col = tileColor;
				for (int j = 0; j < pd->triCount; ++j)
				{
					const unsigned char* t = &tile->detailTris[(pd->triBase + j) * 4];
					for (int k = 0; k < 3; ++k)
					{
						if (t[k] < p->vertCount)
							vertex.push_back({ Core::Maths::Vec3(tile->verts[p->verts[t[k]] * 3], tile->verts[p->verts[t[k]] * 3 + 1], tile->verts[p->verts[t[k]] * 3 + 2]), col });
						else
							vertex.push_back({ Core::Maths::Vec3(tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3], 
																tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3 + 1],
																tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3 + 2]),
																col });
					}
				}
			}
		}

		GLuint VAO, VBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(NavVertex), vertex.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NavVertex), (void*)offsetof(NavVertex, pos));
		glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(NavVertex), (void*)offsetof(NavVertex, color));

		m_VAO = VAO;
		m_VAOSize = static_cast<unsigned>(vertex.size());

		if (!m_shader)
		{
			m_shader = m_engine->GetResourcesManager()->CreateShader("NavMeshShader", "Resources\\Shaders\\NavMeshShader.vert", "Resources\\Shaders\\NavMeshShader.frag");
			if (!m_shader)
				BAKERS_LOG_ERROR("Could not load NavMesh shader");
		}

		glBindVertexArray(0);
	}

	void NavMeshBuilder::DrawNavMesh(const Core::Maths::Mat4& cam, const Core::Maths::Mat4& perspective)
	{
		ZoneScoped
		if (m_VAOSize <= 0)
			return;

		if (!m_shader)
		{
			return;
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_VAO);

		m_shader->UseProgram();
		glUniformMatrix4fv(m_shader->GetLocation("uCam"), 1, GL_TRUE, cam.array);
		glUniformMatrix4fv(m_shader->GetLocation("uProj"), 1, GL_FALSE, perspective.array);
		glDrawArrays(GL_TRIANGLES, 0, m_VAOSize);
		glBindVertexArray(0);
	}

	void NavMeshBuilder::DrawNavMesh(Core::Datastructure::ICamera* cam)
	{
		ZoneScoped
		if (m_navMesh == nullptr)
			return;

		DrawNavMesh(cam->GetCameraMatrix(), cam->GetPerspectiveMatrix());
	}

	bool NavMeshBuilder::SaveNavMesh(const std::string& path) const
	{
		ZoneScoped

		NavMeshSetHeader header;
		header.magic = NAVMESHSET_MAGIC;
		header.version = NAVMESHSET_VERSION;
		header.numTiles = 0;

		FILE* fp;
		fopen_s(&fp, (path + ".nav").c_str(), "wb+");
		if (!fp)
			return false;

		if (!m_navMesh)
		{
			fwrite(&header, sizeof(NavMeshSetHeader), 1, fp);
			fclose(fp);
			return true;
		}

		const dtNavMesh* mesh = m_navMesh;

		for (int i = 0; i < mesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = mesh->getTile(i);
			if (!tile || !tile->header || !tile->dataSize) continue;
			++header.numTiles;
		}
		memcpy(&header.params, mesh->getParams(), sizeof(dtNavMeshParams));
		fwrite(&header, sizeof(NavMeshSetHeader), 1, fp);

		for (int i = 0; i < mesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = mesh->getTile(i);
			if (!tile || !tile->header || !tile->dataSize) continue;

			NavMeshTileHeader tileHeader;
			tileHeader.tileRef = mesh->getTileRef(tile);
			tileHeader.dataSize = tile->dataSize;
			fwrite(&tileHeader, sizeof(tileHeader), 1, fp);

			fwrite(tile->data, tile->dataSize, 1, fp);
		}

		fclose(fp);

		BAKERS_LOG_MESSAGE("Nav Mesh saved");
		return true;
	}

	bool NavMeshBuilder::LoadNavMesh(const std::string& path)
	{
		ZoneScoped
		FILE* fp;
		fopen_s(&fp, (path + ".nav").c_str(), "rb");
		if (!fp) return 0;

		// Read header.
		NavMeshSetHeader header;
		size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return false;
		}
		if (header.magic != NAVMESHSET_MAGIC)
		{
			fclose(fp);
			return false;
		}
		if (header.version != NAVMESHSET_VERSION)
		{
			fclose(fp);
			return false;
		}

		if (m_navMesh)
			dtFreeNavMesh(m_navMesh);
		m_navMesh = dtAllocNavMesh();
		if (!m_navMesh)
		{
			fclose(fp);
			return false;
		}
		dtStatus status = m_navMesh->init(&header.params);
		if (dtStatusFailed(status))
		{
			BuildNavMeshRenderer();
			fclose(fp);
			return 0;
		}
		for (int i = 0; i < header.numTiles; ++i)
		{
			NavMeshTileHeader tileHeader;
			readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
			if (readLen != 1)
			{
				fclose(fp);
				return 0;
			}

			if (!tileHeader.tileRef || !tileHeader.dataSize)
				break;

			unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
			if (!data) break;
			memset(data, 0, tileHeader.dataSize);
			readLen = fread(data, tileHeader.dataSize, 1, fp);
			if (readLen != 1)
			{
				dtFree(data);
				fclose(fp);
				return 0;
			}

			m_navMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		}
		
		fclose(fp);

		if (!m_navQuery.Init(m_navMesh))
		{
			BAKERS_LOG_ERROR("Could not init Detour navmesh query");
			return false;
		}

		BuildNavMeshRenderer();

		return true;
	}
}