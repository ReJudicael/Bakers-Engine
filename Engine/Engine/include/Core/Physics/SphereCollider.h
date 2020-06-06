#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ComponentBase.h"
#include "Collider.h"

namespace Resources
{
	namespace Loader
	{
		class ResourcesManager;
	}
}

namespace Core::Physics
{
	BAKERS_API_CLASS SphereCollider : public Core::Datastructure::ComponentBase, public virtual Collider
	{
	protected:

		/**
		 * Function inheritated from ComponentBase,
		 * override for create the copy as a SphereCollider
		 */
		virtual void StartCopy(IComponent*& copyTo) const override;

		/*
		 * Function inheritated from ComponentBase,
		 * override for create a save of the current for the copyTo
		 */
		virtual void OnCopy(IComponent* copyTo) const override;

		/**
		 * Function inheritated from Collider and ComponentBase,
		 * override for delete the collider from the editor
		 */
		virtual void OnDestroy() override;

		/**
		 * Function inheritated from Collider and ComponentBase,
		 * override for reset the collider,the ComponentBase and
		 * set the SphereCollider to default
		 */
		virtual void OnReset() override;

		/**
		 * Function inheritated from Collider,
		 * set the extent of the SphereCollider to default
		 */
		virtual void SetToDefault() override;

		/**
		 * Function inheritated from Colliderand ComponentBase,
		 * override for start the collider, the componentBase
		 */
		virtual bool OnStart() override;

	public:

		/**
		 * Default constructor
		 */
		SphereCollider() = default;

		/**
		 * Function inheritated from Collider and ComponentBase,
		 * override for init the collider, the ComponentBase and
		 * put the collider in the editor for draw him
		 */
		virtual void OnInit() override;

		/**
		 * Function inheritated from Collider,
		 * override for create a specific shape a box collider
		 * @param scene: the PhysX scene from the PhysicsScene
		 */
		virtual void CreateShape(physx::PxPhysics * physics) override;

		/**
		 * Set the half extent of the capsule collider
		 * @param halfExtent: the half extent we want to give to the shape
		 */
		void SetSphereHalfExtent(float halfExtent);

		/**
		 * Get the half extent of the capsule collider
		 * @return the half extent of the capsule collider
		 */
		float GetSphereHalfExtent() const;

		/**
		 * Function inheritated from Collider,
		 * Draw the collider as a capsule
		 * @param cam: Camera to render to
		 * @param shader: the shader to render to
		 * @param model: the model of the collider
		 */
		virtual void DrawCollider(	Core::Datastructure::ICamera* cam, std::shared_ptr<Resources::Shader> shader,
									std::shared_ptr<Resources::Model> model) override;

		REGISTER_CLASS(Core::Datastructure::ComponentBase, Collider)
	};
}