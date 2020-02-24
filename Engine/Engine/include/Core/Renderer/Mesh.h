#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "Vec3.hpp"
#include "Transform.hpp"
#include "ComponentBase.h"
#include "IRenderable.hpp"


class Mesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
{
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


};

