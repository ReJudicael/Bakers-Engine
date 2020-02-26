#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Vec3.hpp"
#include "Transform.hpp"
#include "ComponentBase.h"
#include "IRenderable.hpp"
#include "Material.h"
#include "OffsetMesh.h"
#include "Vertex.h"
#include "ModelData.h"


class Mesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
{
protected:
	std::vector<std::shared_ptr<Resources::Material>> m_material;
	std::vector<std::shared_ptr<Resources::OffsetMesh>> m_offsetMesh;

	/*std::vector<*/std::shared_ptr<Resources::ModelData>/*>*/ m_modelMesh;
public:

	int		m_vertexCount = 0;

	std::vector<float> m_vertices;
	std::vector<int> m_indices;

	float*	m_projection;

	int		AddTriangle(int v1, int v2, int v3);
	int		AddVertex(float x, float y, float z);

public:
	Mesh();
	~Mesh();

	//virtual void OnStart() override {};
	virtual void OnDestroy() override {};

	void Initialize();

	void SendProjectionMatrix(Core::Maths::Mat4 data);

	virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

	Core::Maths::Mat4 projectionMatrix(float FovY, float Aspect, float Near, float Far);

	void SetMaterial(const int index, std::shared_ptr<Resources::Material> material)
	{ 
		m_material[index] = material; 
	}

	inline void AddMaterial(std::shared_ptr<Resources::Material> material)
	{
		m_material.push_back(material);
	}

	void AddOffsetMesh(const std::shared_ptr<Resources::OffsetMesh> OffsetMesh)
	{
		m_offsetMesh.push_back(OffsetMesh);
		m_offsetMesh[m_offsetMesh.size() - 1]->materialIndices = m_material.size() - 1;
	}

	inline void AddModel(std::shared_ptr<Resources::ModelData> model)
	{
		m_modelMesh = model;
	}
};

