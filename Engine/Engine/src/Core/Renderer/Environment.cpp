#include "Environment.h"
#include "RootObject.hpp"
#include "EngineCore.h"

RTTR_PLUGIN_REGISTRATION
{
	registration::class_<Core::Renderer::Environment>("Environment")
		.constructor()
		.enumeration<Core::Renderer::Environment::ESkyboxType>("Skybox type")
		(
			value("Box", Core::Renderer::Environment::ESkyboxType::BOX),
			value("Sphere", Core::Renderer::Environment::ESkyboxType::SPHERE),
			value("Quads", Core::Renderer::Environment::ESkyboxType::QUAD)
		)
		.property("Type", &Core::Renderer::Environment::m_type)
		.property("Box Tex", &Core::Renderer::Environment::GetBoxTexture, &Core::Renderer::Environment::SetBoxTexture)
		.property("Sphere Tex", &Core::Renderer::Environment::GetSphereTexture, &Core::Renderer::Environment::SetSphereTexture)
		.property("Front Quad Tex", &Core::Renderer::Environment::GetFrontQuadTexture, &Core::Renderer::Environment::SetFrontQuadTexture)
		.property("Right Quad Tex", &Core::Renderer::Environment::GetRightQuadTexture, &Core::Renderer::Environment::SetRightQuadTexture)
		.property("Back Quad Tex", &Core::Renderer::Environment::GetBackQuadTexture, &Core::Renderer::Environment::SetBackQuadTexture)
		.property("Left Quad Tex", &Core::Renderer::Environment::GetLeftQuadTexture, &Core::Renderer::Environment::SetLeftQuadTexture)
		.property("Up Quad Tex", &Core::Renderer::Environment::GetUpQuadTexture, &Core::Renderer::Environment::SetUpQuadTexture)
		.property("Down Quad Tex", &Core::Renderer::Environment::GetDownQuadTexture, &Core::Renderer::Environment::SetDownQuadTexture);

}

namespace Core::Renderer
{
	Environment::Environment() : ComponentBase()
	{}

	Environment::~Environment()
	{
		if (m_box)
			delete m_box;
		if (m_sphere)
			delete m_sphere;

		for (int i{ 0 }; i < 6; ++i)
		{
			if (m_quads[i])
				delete m_quads[i];
		}
	}

	void	Environment::DrawQuads(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj)
	{
		Datastructure::Transform drawTransform = { {0.f, 0.f, -0.5f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} };
		
		const float halfpi = static_cast<float>(M_PI_2);
		const float pi = static_cast<float>(M_PI);

		// Front quad
		m_quads[0]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());

		// Right quad
		drawTransform.SetLocalPos({ 0.5f, 0.f, 0.f });
		drawTransform.SetLocalRot({ 0.f, -halfpi, 0.f });
		m_quads[1]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());

		// Back quad
		drawTransform.SetLocalPos({ 0.f, 0.f, 0.5f });
		drawTransform.SetLocalRot({ 0.f, pi, 0.f });
		m_quads[2]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());

		// Left quad
		drawTransform.SetLocalPos({ -0.5f, 0.f, 0.f });
		drawTransform.SetLocalRot({ 0.f, halfpi, 0.f });
		m_quads[3]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());

		// Up quad
		drawTransform.SetLocalPos({ 0.f, 0.5f, 0.f });
		drawTransform.SetLocalRot({ halfpi, 0.f, 0.f });
		m_quads[4]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());

		// Down quad
		drawTransform.SetLocalPos({ 0.f, -0.5f, 0.f });
		drawTransform.SetLocalRot({ -halfpi, 0.f, 0.f });
		m_quads[5]->DrawFixedMesh(view, proj, drawTransform.GetLocalTrs());
	}

	void	Environment::SetTexturesToQuads()
	{
		std::shared_ptr<Resources::Texture> texture;

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_frontQuadTexture, texture);
		m_quads[0]->SetMainTexture(texture);

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_rightQuadTexture, texture);
		m_quads[1]->SetMainTexture(texture);

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_backQuadTexture, texture);
		m_quads[2]->SetMainTexture(texture);

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_leftQuadTexture, texture);
		m_quads[3]->SetMainTexture(texture);

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_upQuadTexture, texture);
		m_quads[4]->SetMainTexture(texture);

		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_downQuadTexture, texture);
		m_quads[5]->SetMainTexture(texture);
	}

	void	Environment::OnDraw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		if (givenShader) // Skybox has to be drawn with skybox shader only
			return;

		switch (m_type)
		{
		case ESkyboxType::BOX: 
			m_box->DrawFixedMesh(view, proj, Core::Maths::Mat4::Identity());
			break;
		case ESkyboxType::QUAD: 
			DrawQuads(view, proj); 
			break;
		case ESkyboxType::SPHERE: 
			m_sphere->DrawFixedMesh(view, proj, Core::Maths::Mat4::Identity());
			break;
		}
	}

	void	Environment::OnCopy(IComponent* copyTo) const
	{
		ComponentBase::OnCopy(copyTo);
		IRenderable::OnCopy(copyTo);

		Environment* copy{ dynamic_cast<Environment*>(copyTo) };

		copy->m_type = m_type;

		copy->m_boxTexture = m_boxTexture;
		copy->m_sphereTexture = m_sphereTexture;
		copy->m_frontQuadTexture = m_frontQuadTexture;
		copy->m_rightQuadTexture = m_rightQuadTexture;
		copy->m_backQuadTexture = m_backQuadTexture;
		copy->m_leftQuadTexture = m_leftQuadTexture;
		copy->m_upQuadTexture = m_upQuadTexture;
		copy->m_downQuadTexture = m_downQuadTexture;
	}

	void	Environment::StartCopy(IComponent*& copyTo) const
	{
		copyTo = new Environment();
		OnCopy(copyTo);
	}

	bool	Environment::OnStart()
	{
		return IRenderable::OnStart();
	}

    void    Environment::OnDestroy()
    {
		IRenderable::OnDestroy();

		if (m_box)
			m_box->OnDestroy();
		if (m_sphere)
			m_sphere->OnDestroy();

		for (int i{ 0 }; i < 6; ++i)
		{
			if (m_quads[i])
				m_quads[i]->OnDestroy();
		}
    }

    void    Environment::OnReset()
    {
        IRenderable::OnReset();

		if (m_box)
			delete m_box;
		if (m_sphere)
			delete m_sphere;

		for (int i{ 0 }; i < 6; ++i)
		{
			if (m_quads[i])
				delete m_quads[i];
		}

		m_type = ESkyboxType::BOX;
		m_boxTexture = "";
		m_sphereTexture = "";
		m_frontQuadTexture = "";
		m_rightQuadTexture = "";
		m_backQuadTexture = "";
		m_leftQuadTexture = "";
		m_upQuadTexture = "";
		m_downQuadTexture = "";
		OnInit();
    }

	void	Environment::OnInit()
	{
		IRenderable::OnInit();

		m_box = CreateSkyMesh("Skybox");
		m_sphere = CreateSkyMesh("Sphere");

		std::shared_ptr<Resources::Texture> boxTexture;
		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_boxTexture, boxTexture);
		m_box->SetMainTexture(boxTexture);

		std::shared_ptr<Resources::Texture> sphereTexture;
		GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_sphereTexture, sphereTexture);
		m_sphere->SetMainTexture(sphereTexture);

		for (int i{ 0 }; i < 6; ++i)
			m_quads[i] = CreateSkyMesh("Quad");

		SetTexturesToQuads();
	}

	Mesh* Environment::CreateSkyMesh(const char* model)
	{
		Datastructure::EngineCore* core = GetRoot()->GetEngine();

		Mesh* toSet = new Mesh();
		toSet->AddModel(core->GetResourcesManager()->GetModel(model));

		std::shared_ptr<Resources::Material> mat = std::make_shared<Resources::Material>();
		mat->shader = core->GetResourcesManager()->GetShader("Skybox");
		toSet->AddMaterial(mat);

		toSet->OnInit();

		toSet->SetParent(m_parent);
		toSet->SetRoot(GetRoot());

		return toSet;
	}

	void Environment::SetBoxTexture(std::string tex)
	{
		m_boxTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_boxTexture, texture);
			m_box->SetMainTexture(texture);
		}
	};

	void Environment::SetSphereTexture(std::string tex)
	{
		m_sphereTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_sphereTexture, texture);
			m_sphere->SetMainTexture(texture);
		}
	}

	void Environment::SetFrontQuadTexture(std::string tex)
	{
		m_frontQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_frontQuadTexture, texture);
			m_quads[0]->SetMainTexture(texture);
		}
	}

	void Environment::SetRightQuadTexture(std::string tex)
	{
		m_rightQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_rightQuadTexture, texture);
			m_quads[1]->SetMainTexture(texture);
		}
	}

	void Environment::SetBackQuadTexture(std::string tex)
	{
		m_backQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_backQuadTexture, texture);
			m_quads[2]->SetMainTexture(texture);
		}
	}

	void Environment::SetLeftQuadTexture(std::string tex)
	{
		m_leftQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_leftQuadTexture, texture);
			m_quads[3]->SetMainTexture(texture);
		}
	}

	void Environment::SetUpQuadTexture(std::string tex)
	{
		m_upQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_upQuadTexture, texture);
			m_quads[4]->SetMainTexture(texture);
		}
	}

	void Environment::SetDownQuadTexture(std::string tex)
	{
		m_downQuadTexture = tex;

		if (IsInit())
		{
			std::shared_ptr<Resources::Texture> texture;
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_downQuadTexture, texture);
			m_quads[5]->SetMainTexture(texture);
		}
	}
}