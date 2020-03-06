#include "WidgetScene.h"

namespace Editor::Widget
{
	WidgetScene::WidgetScene() :
		IWidget(W_SCENE)
	{
	}

	void WidgetScene::Tick()
	{
		ImGui::Text(W_SCENE);
	}
}
