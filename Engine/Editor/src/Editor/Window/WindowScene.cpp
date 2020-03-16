#include "WindowScene.h"

namespace Editor::Window
{
	WindowScene::WindowScene(bool visible) :
		AWindow{ "Scene" }
	{
		isVisible = visible;
	}

	void WindowScene::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
