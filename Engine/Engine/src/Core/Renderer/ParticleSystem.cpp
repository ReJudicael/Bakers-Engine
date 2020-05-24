#include "ParticleSystem.h"
#include "EngineCore.h"
#include "RootObject.hpp"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Renderer::ParticleSystem>("Particle System")
		.constructor()
		.property("Active", &Core::Renderer::ParticleSystem::m_active)
		.property("Particle texture", &Core::Renderer::ParticleSystem::GetParticleTexture, &Core::Renderer::ParticleSystem::SetParticleTexture)
		.property("Birth Color", &Core::Renderer::ParticleSystem::m_birthColor)
		.property("Death Color", &Core::Renderer::ParticleSystem::m_deathColor)
		.property("Particles number", &Core::Renderer::ParticleSystem::m_maxParticlesNb)
		.property("Time Before Spawn", &Core::Renderer::ParticleSystem::m_timeBeforeSpawn)
		.property("Life time", &Core::Renderer::ParticleSystem::m_particlesLifeTime)
		.property("Gravity", &Core::Renderer::ParticleSystem::m_gravity);
}

namespace Core::Renderer
{
	ParticleSystem::ParticleSystem() : ComponentUpdatable()
	{}

	ParticleSystem::~ParticleSystem()
	{
		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].mesh)
				delete m_particles[i].mesh;
		}
	}

	void	ParticleSystem::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
			ComponentUpdatable::OnCopy(copyTo);
		IRenderable::OnCopy(copyTo);

		ParticleSystem* copy{ dynamic_cast<ParticleSystem*>(copyTo) };

		copy->m_particles = m_particles;
		copy->m_maxParticlesNb = m_maxParticlesNb;
		copy->m_particlesLifeTime = m_particlesLifeTime;
		copy->m_timeBeforeSpawn = m_timeBeforeSpawn;
		copy->m_currentTime = m_currentTime;
		copy->m_gravity = m_gravity;
		copy->m_birthColor = m_birthColor;
		copy->m_deathColor = m_deathColor;
		copy->m_particleTexture = m_particleTexture;
	}

	void	ParticleSystem::StartCopy(IComponent*& copyTo) const
	{
		ZoneScoped
			copyTo = new ParticleSystem();
		OnCopy(copyTo);
	}

	bool	ParticleSystem::OnStart()
	{
		ZoneScoped
			ComponentUpdatable::OnStart();
			return IRenderable::OnStart();
	}

	void    ParticleSystem::OnDestroy()
	{
		IRenderable::OnDestroy();
		ComponentUpdatable::OnDestroy();

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].mesh)
				m_particles[i].mesh->OnDestroy();
		}
	}

	void    ParticleSystem::OnReset()
	{
		ComponentUpdatable::OnReset();
		IRenderable::OnReset();

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].mesh)
				delete m_particles[i].mesh;
		}

		m_particles.clear();

		m_maxParticlesNb = 10;
		m_particlesLifeTime = 2.f;
		m_timeBeforeSpawn = 0.2f;
		m_currentTime = 0.f;
		m_gravity = 0.f;
		
		m_birthColor = { 1, 1, 1, 1 };
		m_deathColor = { 1, 1, 1, 1 };
		SetParticleTexture("");
		
		OnInit();
	}

	void	ParticleSystem::OnInit()
	{
		IRenderable::OnInit();
		ComponentUpdatable::OnInit();

		CreateParticle();
	}

	void ParticleSystem::OnUpdate(float deltaTime)
	{
		if (!m_active)
			return;

		if (m_currentTime > 0)
			m_currentTime -= deltaTime;
		else
		{
			if (m_particles.size() < m_maxParticlesNb)
				CreateParticle();
			else if (ResetParticle())
				m_currentTime = m_timeBeforeSpawn;
		}

		// Update particles
		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].lifeTime > 0)
			{
				m_particles[i].lifeTime -= deltaTime;
				if (m_particles[i].lifeTime < 0)
					m_particles[i].lifeTime = 0;
				float ratio = m_particles[i].lifeTime / m_particlesLifeTime;
				m_particles[i].color = m_birthColor * ratio + m_deathColor * (1 - ratio);
				m_particles[i].position += m_particles[i].movement * deltaTime * 1;
				m_particles[i].movement.y -= m_gravity * deltaTime;
			}
		}
	}

	void	ParticleSystem::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		if (!m_active)
			return;

		Core::Maths::Mat4 transform = m_parent->GetGlobalTRS();
		Core::Maths::Vec3 particleScale = m_parent->GetGlobalScale() * 0.1;

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		if (m_particles.size() > 0)
		{
			m_particles[0].GetShader()->UseProgram();
			glUniform3fv(m_particles[0].GetShader()->GetLocation("uScale"), 1, particleScale.xyz);
		}

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].lifeTime > 0)
			{
				transform(0, 3) += m_particles[i].position.x * particleScale.x;
				transform(1, 3) += m_particles[i].position.y * particleScale.y;
				transform(2, 3) += m_particles[i].position.z * particleScale.z;
				glUniform4fv(m_particles[i].GetShader()->GetLocation("uParticleColor"), 1, m_particles[i].color.rgba);
				m_particles[i].mesh->DrawFixedMesh(view, proj, transform);
				transform(0, 3) -= m_particles[i].position.x * particleScale.x;
				transform(1, 3) -= m_particles[i].position.y * particleScale.y;
				transform(2, 3) -= m_particles[i].position.z * particleScale.z;
			}
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Core::Maths::Vec3 ParticleSystem::GenerateRandomMovement()
	{
		float x = (float(rand() % 201) / 100.f) - 1.f;
		float z = (float(rand() % 201) / 100.f) - 1.f;

		return Core::Maths::Vec3(x, 1, z).Normalized();
	}

	void ParticleSystem::CreateParticle()
	{
		Particle newParticle;
		
		newParticle.movement = GenerateRandomMovement();
		newParticle.color = m_birthColor;
		newParticle.lifeTime = m_particlesLifeTime;

		Datastructure::EngineCore* core = GetRoot()->GetEngine();

		Mesh* particleDisplay = new Mesh();
		particleDisplay->AddModel(core->GetResourcesManager()->GetModel("Quad"));

		std::shared_ptr<Resources::Material> mat = std::make_shared<Resources::Material>();
		mat->shader = core->GetResourcesManager()->GetShader("Particle");
		particleDisplay->AddMaterial(mat);

		std::shared_ptr<Resources::Texture> texture;
		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_particleTexture, texture);
		particleDisplay->SetMainTexture(texture);

		particleDisplay->OnInit();

		particleDisplay->SetParent(m_parent);
		particleDisplay->SetRoot(GetRoot());

		newParticle.mesh = particleDisplay;

		m_particles.push_back(newParticle);
	}

	bool ParticleSystem::ResetParticle()
	{
		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].lifeTime <= 0)
			{
				m_particles[i].lifeTime = m_particlesLifeTime;
				m_particles[i].position = { 0, 0, 0 };
				m_particles[i].movement = GenerateRandomMovement();
				m_particles[i].color = m_birthColor;
				return true;
			}
		}

		return false; // No particle with lifeTime over has been found
	}

	void ParticleSystem::SetParticleTexture(const std::string& tex)
	{
		m_particleTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_particleTexture, texture);
			for (size_t i{0}; i < m_particles.size(); ++i)
				m_particles[i].mesh->SetMainTexture(texture);
		}
	}
}