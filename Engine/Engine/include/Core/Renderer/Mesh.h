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
#include "CoreMinimal.h"

namespace Resources
{
	struct Model;
	struct Material;
	namespace Loader
	{
		class ResourcesManager;
	}

}

BAKERS_API_CLASS Mesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
{
protected:
	std::shared_ptr<Resources::Model> m_model;

	std::vector<std::shared_ptr<Resources::Material>> m_materialsModel;

	virtual void	OnReset() override;
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

	virtual void OnDestroy() override {};

	void Initialize();

	void SendProjectionMatrix(Core::Maths::Mat4 data);

	bool IsModelLoaded();

	void CreateAABBMesh();

	virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

	Core::Maths::Mat4 projectionMatrix(float FovY, float Aspect, float Near, float Far);

	int					GetVertexCount();

	inline void AddModel(std::shared_ptr<Resources::Model> model)
	{
		m_model = model;
	}

	void AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial);

	REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IRenderable)
};

