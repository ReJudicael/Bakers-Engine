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
		 * @param cam: Camera to pass to OnDraw
		 */
		void DrawQuads(Datastructure::ICamera* cam);

		/**
		 * Set texture set from editor to each quad
		 */
		void SetTexturesToQuads();

	protected:

		/**
		 * Called for rendering. Must be overriden if the component is inherited
		 * @param cam: Camera to render to
		 */
		virtual void OnDraw(Datastructure::ICamera* cam);

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

