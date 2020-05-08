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
		.property("Box Tex", &Core::Renderer::Environment::m_boxTexture)
		.property("Sphere Tex", &Core::Renderer::Environment::m_sphereTexture)
		.property("Front Quad Tex", &Core::Renderer::Environment::m_frontQuadTexture)
		.property("Right Quad Tex", &Core::Renderer::Environment::m_rightQuadTexture)
		.property("Back Quad Tex", &Core::Renderer::Environment::m_backQuadTexture)
		.property("Left Quad Tex", &Core::Renderer::Environment::m_leftQuadTexture)
		.property("Up Quad Tex", &Core::Renderer::Environment::m_upQuadTexture)
		.property("Down Quad Tex", &Core::Renderer::Environment::m_downQuadTexture);

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

	void	Environment::DrawQuads(Datastructure::ICamera* cam)
	{
		Datastructure::Transform drawTransform = { {0.f, 0.f, -0.5f}, {0.f, 0.f, 0.f}, {1.f, 1.f, 1.f} };
		
		const float halfpi = static_cast<float>(M_PI_2);
		const float pi = static_cast<float>(M_PI);

		// Front quad
		m_quads[0]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());

		// Right quad
		drawTransform.SetLocalPos({ 0.5f, 0.f, 0.f });
		drawTransform.SetLocalRot({ 0.f, -halfpi, 0.f });
		m_quads[1]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());

		// Back quad
		drawTransform.SetLocalPos({ 0.f, 0.f, 0.5f });
		drawTransform.SetLocalRot({ 0.f, pi, 0.f });
		m_quads[2]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());

		// Left quad
		drawTransform.SetLocalPos({ -0.5f, 0.f, 0.f });
		drawTransform.SetLocalRot({ 0.f, halfpi, 0.f });
		m_quads[3]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());

		// Up quad
		drawTransform.SetLocalPos({ 0.f, 0.5f, 0.f });
		drawTransform.SetLocalRot({ halfpi, 0.f, 0.f });
		m_quads[4]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());

		// Down quad
		drawTransform.SetLocalPos({ 0.f, -0.5f, 0.f });
		drawTransform.SetLocalRot({ -halfpi, 0.f, 0.f });
		m_quads[5]->DrawFixedMesh(cam, drawTransform.GetLocalTrs());
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

	void	Environment::OnDraw(Datastructure::ICamera* cam)
	{
		std::shared_ptr<Resources::Texture> texture;
		
		switch (m_type)
		{
		case ESkyboxType::BOX: 
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_boxTexture, texture);
			m_box->SetMainTexture(texture);
			m_box->DrawFixedMesh(cam, Core::Maths::Mat4::Identity()); 
			break;
		case ESkyboxType::QUAD: 
			SetTexturesToQuads();
			DrawQuads(cam); 
			break;
		case ESkyboxType::SPHERE: 
			GetRoot()->GetEngine()->GetResourcesManager()->LoadTexture(m_sphereTexture, texture);
			m_sphere->SetMainTexture(texture);
			m_sphere->DrawFixedMesh(cam, Core::Maths::Mat4::Identity()); 
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

		for (int i{ 0 }; i < 6; ++i)
			m_quads[i] = CreateSkyMesh("Quad");
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
}