#include "IRenderable.hpp"
#include "RootObject.hpp"
#include "CoreMinimal.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<IRenderable>("IRenderable")
		.property("VAO", &IRenderable::m_VAO);
}

namespace Core::Datastructure
{

	void Core::Datastructure::IRenderable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddRenderable(this);
	}

	void IRenderable::Draw(ICamera* cam)
	{
		ZoneText("Rendering single component")
			TracyGpuZone("Rendering single component")
		
		if (IsInit() && m_isActive)
		{
			OnDraw(cam);
		}
	}
}
