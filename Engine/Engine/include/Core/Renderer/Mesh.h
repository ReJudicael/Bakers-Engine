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

public:
	Mesh();
	~Mesh();

	/**
	 * Function inheritated from IRenderable,
	 */
	virtual void OnDestroy() override {};

	/**
	 * Send the projection matrix to m_projection of the mesh
	 * @param data: the projection matrix
	 */
	void SendProjectionMatrix(Core::Maths::Mat4 data);

	/**
	 * Check if the model is load
	 */
	bool IsModelLoaded();

	/**
	 * Create the AABB box of the mesh in the physics scene
	 */
	void CreateAABBMesh();

	/**
	 * Function inheritated from IRenderable,
	 * override for draw the mesh with the material and the model
	 * @param cam: the camera to render to
	 */
	virtual void OnDraw(Core::Datastructure::ICamera* cam) override;

	/**
	 * get the number total of the vertex in the mesh
	 * @return the count of vertex
	 */
	int					GetVertexCount();

	/**
	 * set the model of the mesh
	 * @param model: the model we want to link to the mesh
	 */
	inline void AddModel(std::shared_ptr<Resources::Model> model)
	{
		m_model = model;
	}

	/**
	 * add all the materials of the mesh
	 * @param resources: the resourcesmanager of the scene
	 * @param namesMaterial: all the names of material we want to link to the mesh
	 */
	void AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial);

	REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IRenderable)
};

