#include "IRenderable.hpp"
#include "RootObject.hpp"
#include "Debug.h"

namespace Core::Datastructure
{

	void Core::Datastructure::IRenderable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddRenderable(this);
	}

	void Core::Datastructure::IRenderable::Draw(const std::list<ICamera*>& cameras)
	{
		ZoneScoped
			ZoneText("Render of a component", 22)
		TracyGpuZone("Rendering single component")
		if (IsInit() && m_isActive)
		{
			for (auto it{ cameras.begin() }; it != cameras.end(); ++it)
				OnDraw(*it);
		}
	}
}
