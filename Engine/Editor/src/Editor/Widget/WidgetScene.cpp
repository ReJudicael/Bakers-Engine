#include "WidgetScene.h"

namespace Editor::Widget
{
	WidgetScene::WidgetScene() :
		AWidget("Scene")
	{
	}

	void WidgetScene::Tick()
	{
		ImGui::Text(GetNameID().c_str());
	}
}
