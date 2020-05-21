#include "IRenderable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	using namespace Core::Datastructure;
	registration::class_<IRenderable>("IRenderable")
		.property_readonly("VAO", &IRenderable::m_VAO, rttr::detail::protected_access());
}

namespace Core::Datastructure
{

	void Core::Datastructure::IRenderable::OnInit()
	{
		if (GetRoot() == nullptr)
			return;
		GetRoot()->AddRenderable(this);
		IComponent::OnInit();
	}
	IRenderable::~IRenderable()
	{
		if (GetRoot() == nullptr)
			return;
		GetRoot()->RemoveRenderable(this);
	}

	void IRenderable::Draw(const Core::Maths::Mat4& view, const Core::Maths::Mat4& proj, std::shared_ptr<Resources::Shader> givenShader)
	{
		ZoneScoped
		ZoneText("Rendering single component", 27)
			TracyGpuZone("Rendering single component")
		
		if (IsInit() && m_isActive && !IsDestroyed() && m_parent->IsActive())
		{
			OnDraw(view, proj, givenShader);
		}
	}

	void IRenderable::OnCopy(IComponent* copyTo) const
	{
		ZoneScoped
		IComponent::OnCopy(copyTo);
		IRenderable* copy{ dynamic_cast<IRenderable*>(copyTo) };

		copy->m_texture = m_texture;
		copy->m_VAO = m_VAO;
	}

	void IRenderable::OnReset()
	{
		IComponent::OnReset();
		GetRoot()->RemoveRenderable(this);
	}
}
