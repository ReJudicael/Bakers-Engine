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
#include "Model.h"
#include "CoreMinimal.h"

namespace physx
{
	class PxScene;
}

namespace Resources
{
	struct Material;
	namespace Loader
	{
		class ResourcesManager;
	}

}
namespace Core::Renderer
{
	BAKERS_API_CLASS Mesh : public Core::Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
	{
	protected:
		std::shared_ptr<Resources::Model> m_model;

		std::string	m_modelName;

		bool		m_isRoot{ false };
		bool		m_isChild{ false };
		bool		m_castShadow{ true };

		std::vector<std::shared_ptr<Resources::Material>>	m_materialsModel;
		std::vector<std::string>							m_materialsNames;

		Core::SystemManagement::EventSystem<>				m_destroyActorEvent;
		std::vector<std::function<void()>>					m_destroyMaterialEvent;

		virtual void	OnReset() override;
		virtual void	OnCopy(IComponent* copyTo) const override;
		virtual void	StartCopy(IComponent*& copyTo) const override;
		virtual bool	OnStart() override;

		virtual void	UpdateModel();
	public:

		int		m_vertexCount = 0;

	protected:
		bool ChangeOneMaterial(std::string newMaterial, int iD);

		void MaterialSendToOpengGL(	const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, float* trs,
									std::shared_ptr<Resources::Material> mat, std::shared_ptr<Resources::Shader> shader);

	public:

		std::string					GetModel() { return m_modelName; };
		void						SetModel(std::string newModel);

		std::vector<std::string>	GetMaterials() { return m_materialsNames; }
		void						SetMaterials(std::vector<std::string> newMaterials);

		void						SetChildModel(std::string newModel);

		float* m_projection;

	public:
		Mesh();
		~Mesh();

		/**
		 * Function inheritated from IRenderable,
		 */
		virtual void OnDestroy() override;

		virtual void OnInit() override;

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
		 * @param scene: the physx scene to put the AABB mesh
		 */
		bool CreateAABBMesh(physx::PxScene*& scene);

		inline Core::Maths::Vec3 GetAABBMaxModel()
		{
			return m_model->max;
		}
		inline Core::Maths::Vec3 GetAABBMinModel()
		{
			return m_model->min;
		}
		/**
		 * Update the mesh namesMaterial
		 * @param index: the index of the material in this mesh
		 * @param newName: the new name of the material
		 */
		void UpdateNameMaterial(const int index, const std::string& newName)
		{
			m_materialsNames[index] = newName;
		}

		/**
		 * Function inheritated from IRenderable,
		 * override for draw the mesh with the material and the model
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param givenShader: Shader to use instead of material shader if provided
		 */
		virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr) override;

		/**
		 * Handle OpenGL drawing of mesh
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param trs: Model matrix array to send to shader
		 * @param givenShader: Shader to use instead of material shader if provided
		 */
		void Display(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, float* trs, std::shared_ptr<Resources::Shader> givenShader = nullptr);

		void FindMaterialToDelete(const Resources::OffsetMesh& currOffsetMesh);

		/**
		 * Draw mesh with given transform instead of parent transform
		 * Used to draw environment meshes for skybox
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param trs: Model matrix used for shader instead of parent transform
		 */
		void DrawFixedMesh(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, Core::Maths::Mat4 trs);

		/**
		 * Adds current mesh to NavMesh
		 */
		void		AddToNavMesh();

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
		 * Add all the materials of the mesh
		 * @param resources: the resourcesmanager of the scene
		 * @param namesMaterial: all the names of material we want to link to the mesh
		 */
		virtual void AddMaterials(Resources::Loader::ResourcesManager& resources, const std::vector<std::string>& namesMaterial);

		/**
		 * Add material to mesh
		 * @param material: Material to add
		 */
		inline void AddMaterial(std::shared_ptr<Resources::Material> material)
		{
			m_materialsModel.push_back(material);
		}
		/**
		 * Delete the event attached the material
		 * @param indexMaterial: the index of the material
		 * @param indexEvent: the index of the event in the map of EventSystem
		 */
		inline void DeleteMaterialEvent(const int indexMaterial, const int indexEvent)
		{
			m_materialsModel[indexMaterial]->UpdateNameMaterial.RemoveListener(indexEvent);
		}

		/**
		 * Get first material of materials list
		 * @return Shared ptr to material or nullptr if list is empty
		 */
		std::shared_ptr<Resources::Material> GetMainMaterial();

		/**
		 * Set new value for first material of materials list
		 * @param material: Material to set
		 */
		void SetMainMaterial(std::shared_ptr<Resources::Material> material);

		/**
		 * Set texture to given material
		 * @param texture: New texture to set
		 * @parem material: Id of the material that will be updated
		 */
		void SetMainTexture(std::shared_ptr<Resources::Texture> texture, int material = 0);

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Core::Datastructure::IRenderable)
	};
}

