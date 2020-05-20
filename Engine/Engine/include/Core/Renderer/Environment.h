#pragma once

#include "ComponentBase.h"
#include "CoreMinimal.h"
#include "Mesh.h"
#include "IRenderable.hpp"
#include "ICamera.h"

namespace Core::Renderer
{
	// Class handling Skybox
	BAKERS_API_CLASS Environment : public Datastructure::ComponentBase, public virtual Core::Datastructure::IRenderable
	{
	public:
		// Represent mesh used to display skybox
		enum class ESkyboxType
		{
			BOX = 0,
			SPHERE,
			QUAD
		};

		bool		IsBox() const;
		bool		IsSphere() const;
		bool		IsQuad() const;

	private:
		ESkyboxType m_type{ ESkyboxType::BOX };

		// Sky meshes
		Mesh* m_box{ nullptr };
		Mesh* m_sphere{ nullptr };
		Mesh* m_quads[6];

		// Sky textures
		std::string m_boxTexture{ "" };
		std::string m_sphereTexture{ "" };

		std::string m_frontQuadTexture{ "" };
		std::string m_rightQuadTexture{ "" };
		std::string m_backQuadTexture{ "" };
		std::string m_leftQuadTexture{ "" };
		std::string m_upQuadTexture{ "" };
		std::string m_downQuadTexture{ "" };

		/**
		 * Create and init mesh
		 * @param model: Model name to create mesh
		 */
		Mesh* CreateSkyMesh(const char* model);

		/**
		 * OnDraw call for each skybox quad
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 */
		void DrawQuads(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj);

		/**
		 * Set texture set from editor to each quad
		 */
		void SetTexturesToQuads();

		// Setters for texture registration
		void SetBoxTexture(std::string tex);
		void SetSphereTexture(std::string tex);
		void SetFrontQuadTexture(std::string tex);
		void SetRightQuadTexture(std::string tex);
		void SetBackQuadTexture(std::string tex);
		void SetLeftQuadTexture(std::string tex);
		void SetUpQuadTexture(std::string tex);
		void SetDownQuadTexture(std::string tex);

		// Getters for texture registration
		std::string GetBoxTexture() { return m_boxTexture; };
		std::string GetSphereTexture() { return m_sphereTexture; };
		std::string GetFrontQuadTexture() { return m_frontQuadTexture; };
		std::string GetRightQuadTexture() { return m_rightQuadTexture; };
		std::string GetBackQuadTexture() { return m_backQuadTexture; };
		std::string GetLeftQuadTexture() { return m_leftQuadTexture; };
		std::string GetUpQuadTexture() { return m_upQuadTexture; };
		std::string GetDownQuadTexture() { return m_downQuadTexture; };

	protected:

		/**
		 * Called for rendering. Must be overriden if the component is inherited
		 * @param view: View matrix
		 * @param proj: Projection matrix
		 * @param givenShader: Shader to use instead of material shader if provided
		 */
		virtual void OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader = nullptr);

		/**
		 * Copies the data of the component into the given component.
		 * Should always be safe to cast pointer to current component type.
		 */
		virtual void	OnCopy(IComponent* copyTo) const override;
		/**
		 * Copies the component in the given pointer.
		 * On override, should not call other versions of the function.
		 */
		virtual void	StartCopy(IComponent*& copyTo) const override;

		/**
		 * First frame upon creation event
		 */
		virtual bool	OnStart() override;

		/**
		 * Destroy event
		 */
		virtual void	OnDestroy() override;

		/**
		 * Reset event
		 */
		virtual void	OnReset() override;
	public:
		/**
		 * Default Constructor
		 */
		Environment();

		/*
		 * Destructor
		 */
		~Environment();

		/**
		 * Init event
		 */
		void	OnInit() override;

		REGISTER_CLASS(ComponentBase, IRenderable)

	};
}

