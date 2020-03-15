#include "WindowScene.h"

namespace Editor::Window
{
	WindowScene::WindowScene() :
		AWindow("Scene")
	{
	}

	void WindowScene::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
