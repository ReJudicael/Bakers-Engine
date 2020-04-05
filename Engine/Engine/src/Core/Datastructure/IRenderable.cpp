#include "IRenderable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<IRenderable>("IRenderable")
		.property_readonly("VAO", &IRenderable::m_VAO);
}

namespace Core::Datastructure
{

	void Core::Datastructure::IRenderable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddRenderable(this);
	}
	IRenderable::~IRenderable()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->RemoveRenderable(this);
	}

	void IRenderable::Draw(ICamera* cam)
	{
		ZoneScoped
		ZoneText("Rendering single component", 27)
			TracyGpuZone("Rendering single component")
		
		if (IsInit() && m_isActive && !IsDestroyed())
		{
			OnDraw(cam);
		}
	}

	void IRenderable::OnCopy(IComponent* copyTo) const
	{
		IComponent::OnCopy(copyTo);
		IRenderable* copy{ dynamic_cast<IRenderable*>(copyTo) };

		copy->m_texture = m_texture;
		copy->m_VAO = m_VAO;
	}

	void IRenderable::OnReset()
	{
		IComponent::OnReset();
		GetScene()->RemoveRenderable(this);
	}
}
