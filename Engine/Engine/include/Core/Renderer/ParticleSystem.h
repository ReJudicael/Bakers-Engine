#pragma once

#include "CoreMinimal.h"
#include "ComponentUpdatable.h"
#include "IRenderable.hpp"
#include "Vec3.hpp"
#include "Color.hpp"
#include "Mesh.h"

namespace Core::Renderer
{
	// Class handling particles creation and display
	BAKERS_API_CLASS ParticleSystem : public Datastructure::ComponentUpdatable, public virtual Core::Datastructure::IRenderable
	{
		// Store data for a single particule
		struct Particle
		{
			Mesh* mesh;
			Maths::Vec3		position;
			Maths::Vec3		movement;
			Maths::Color	color;
			float			lifeTime;

			/**
			 * Give access to shader used by particle quad (called to send particle color to fragment shader)
			 * @return Shader of first material of particle mesh
			 */
			std::shared_ptr<Resources::Shader> GetShader() { return mesh->GetMainMaterial()->shader; };
		};

		bool m_active{ true };
		std::vector<Particle> m_particles;
		int m_maxParticlesNb{ 10 };
		Maths::Vec3 m_spawnZone;

		float m_particlesLifeTime{ 2.f };
		float m_timeBeforeSpawn{ 0.2f };
		float m_currentTime{ 0.f };
		float m_gravity{ 0.f };
		float m_particleSpeed{ 0.1f };

		Maths::Vec2	m_dispersion{ 0.5f, 0.5f };

		Maths::Color	m_birthColor;
		Maths::Color	m_deathColor;

		std::string	m_particleTexture{ "" };
		
		/**
		 * Generate random position for particle
		 * @return Initial position to give to a particle
		 */
		Core::Maths::Vec3 GenerateRandomPosition();

		/**
		 * Generate random movement vector for particle
		 * @return Initial movement to give to a particle
		 */
		Core::Maths::Vec3 GenerateRandomMovement();

		/**
		 * Create new particle and add it to particles vector
		 */
		void CreateParticle();

		/**
		 * Search for first particle inactive and reset it
		 * @return True if a particle has been reset, false if every particle was already active
		 */
		bool ResetParticle();

		/**
		 * Particle texture setter for registration
		 * @param tex: Path to the new texture
		 */
		void SetParticleTexture(const std::string& tex);
		
		/**
		 * Particle texture getter for registration
		 * @return Path of the current texture used for particles
		 */
		const std::string& GetParticleTexture() { return m_particleTexture; };

		protected:
			/**
			 * Called for rendering. Must be overriden if the component is inherited
			 * @param view: View matrix
			 * @param proj: Projection matrix
			 * @param givenShader: Shader to use instead of material shader if provided
			 */
			virtual void OnDraw(const Core::Maths::Mat4 & view, const Core::Maths::Mat4 & proj, std::shared_ptr<Resources::Shader> givenShader = nullptr);

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
		ParticleSystem();

		/**
		 * Destructor
		 */
		~ParticleSystem();

		/**
		 * Init event
		 */
		void	OnInit() override;

		/**
		 * Call Update function in Lua script
		 * @param deltaTime: Time elapsed between two frames, sent to Lua Update
		 */
		virtual void OnUpdate(float deltaTime) override;

		/**
		 * Activate or deactivate the particle system
		 * @param active: True to activate the particles, false to deactive them
		 */
		void Activate(bool active = true) { m_active = active; };

		REGISTER_CLASS(ComponentUpdatable, IRenderable)
	};
}