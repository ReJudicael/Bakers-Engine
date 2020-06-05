#include "ParticleSystem.h"
#include "EngineCore.h"
#include "RootObject.hpp"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	registration::class_<Core::Renderer::ParticleSystem>("Particle System")
		.constructor()
		.property("Active", &Core::Renderer::ParticleSystem::m_active)
		.property("Particle texture", &Core::Renderer::ParticleSystem::GetParticleTexture, &Core::Renderer::ParticleSystem::SetParticleTexture)
		.property("Birth Color", &Core::Renderer::ParticleSystem::m_birthColor)
		.property("Death Color", &Core::Renderer::ParticleSystem::m_deathColor)
		.property("Particles number", &Core::Renderer::ParticleSystem::m_maxParticlesNb)
		.property("Spawn zone", &Core::Renderer::ParticleSystem::m_spawnZone)
		.property("Time Before Spawn", &Core::Renderer::ParticleSystem::m_timeBeforeSpawn)
		.property("Life time", &Core::Renderer::ParticleSystem::m_particlesLifeTime)
		.property("Gravity", &Core::Renderer::ParticleSystem::m_gravity)
		.property("Speed", &Core::Renderer::ParticleSystem::m_particleSpeed)
		.property("Dispersion", &Core::Renderer::ParticleSystem::m_dispersion)
		.property("Loop", &Core::Renderer::ParticleSystem::m_loop);
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

		copy->m_active = m_active;
		copy->m_loop = m_loop;
		copy->m_particles = m_particles;
		copy->m_maxParticlesNb = m_maxParticlesNb;
		copy->m_spawnZone = m_spawnZone;
		copy->m_particlesLifeTime = m_particlesLifeTime;
		copy->m_timeBeforeSpawn = m_timeBeforeSpawn;
		copy->m_currentTime = m_currentTime;
		copy->m_gravity = m_gravity;
		copy->m_particleSpeed = m_particleSpeed;
		copy->m_dispersion = m_dispersion;
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
		
		IComponent::OnReset();
		GetRoot()->RemoveRenderable(this, 1);

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].mesh)
				delete m_particles[i].mesh;
		}

		m_particles.clear();

		m_active = true;
		m_loop = true;
		m_maxParticlesNb = 10;
		m_spawnZone = { 0, 0, 0 };
		m_particlesLifeTime = 2.f;
		m_timeBeforeSpawn = 0.2f;
		m_currentTime = 0.f;
		m_gravity = 0.f;
		m_particleSpeed = 0.1f;
		m_dispersion = { 0.5f, 0.5f };
		
		m_birthColor = { 1, 1, 1, 1 };
		m_deathColor = { 1, 1, 1, 1 };
		SetParticleTexture("");
		
		OnInit();
	}

	void	ParticleSystem::OnInit()
	{
		if (GetRoot() == nullptr)
			return;
		GetRoot()->AddRenderable(this, 1);
		IComponent::OnInit();

		ComponentUpdatable::OnInit();
	}

	void ParticleSystem::OnUpdate(float deltaTime)
	{
		if (m_currentTime > 0)
			m_currentTime -= deltaTime;
		else if (m_active)
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
				m_particles[i].position += m_particles[i].movement * deltaTime * m_particleSpeed;
				m_particles[i].movement.y -= m_gravity * deltaTime * m_parent->GetGlobalScale().y;
			}
		}
	}

	void	ParticleSystem::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		if (givenShader)
			return;

		Core::Maths::Mat4 transform = m_parent->GetGlobalTRS();
		Core::Maths::Vec3 particleScale = m_parent->GetGlobalScale() * 0.1f;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);
		if (m_particles.size() > 0)
		{
			m_particles[0].GetShader()->UseProgram();
			glUniform3fv(m_particles[0].GetShader()->GetLocation("uScale"), 1, particleScale.xyz);
		}

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].lifeTime > 0)
			{
				transform(0, 3) = m_particles[i].position.x;
				transform(1, 3) = m_particles[i].position.y;
				transform(2, 3) = m_particles[i].position.z;
				glUniform4fv(m_particles[i].GetShader()->GetLocation("uParticleColor"), 1, m_particles[i].color.rgba);
				m_particles[i].mesh->DrawFixedMesh(view, proj, transform);
			}
		}

		glDepthMask(GL_TRUE);
	}

	Core::Maths::Vec3 ParticleSystem::GenerateRandomPosition()
	{
		float x = m_parent->GetGlobalScale().x * ((float(rand() % int(m_spawnZone.x * 200 + 1))) / 100.f - m_spawnZone.x);
		float y = m_parent->GetGlobalScale().y * ((float(rand() % int(m_spawnZone.y * 200 + 1))) / 100.f - m_spawnZone.y);
		float z = m_parent->GetGlobalScale().z * ((float(rand() % int(m_spawnZone.z * 200 + 1))) / 100.f - m_spawnZone.z);

		return m_parent->GetGlobalPos() + Maths::Vec3(x, y, z);
	}

	Core::Maths::Vec3 ParticleSystem::GenerateRandomMovement()
	{
		Maths::Vec3 up = m_parent->Up();
		float x = float(rand() % int(m_dispersion.x * 200 + 1)) / 100.f - m_dispersion.x;
		float z = float(rand() % int(m_dispersion.y * 200 + 1)) / 100.f - m_dispersion.y;
		Maths::Quat RotateX = Maths::Quat::AngleAxis(x, m_parent->Right());
		Maths::Quat RotateZ = Maths::Quat::AngleAxis(z, m_parent->Forward());
		Maths::Quat FullRotation = RotateX * RotateZ;

		return FullRotation.Rotate(up) * m_parent->GetGlobalScale();
	}

	void ParticleSystem::CreateParticle()
	{
		Particle newParticle;
		
		newParticle.position = GenerateRandomPosition();
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
		if (!m_loop)
			return false;

		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].lifeTime <= 0)
			{
				m_particles[i].lifeTime = m_particlesLifeTime;
				m_particles[i].position = GenerateRandomPosition();
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

	void ParticleSystem::Restart()
	{
		m_active = true;
		Clear();
	}

	void ParticleSystem::Clear()
	{
		for (size_t i{ 0 }; i < m_particles.size(); ++i)
		{
			if (m_particles[i].mesh)
				delete m_particles[i].mesh;
		}

		m_particles.clear();
	}
}