#include "WidgetScene.h"

namespace Editor::Widget
{
	WidgetScene::WidgetScene() :
		IWidget("Scene")
	{
	}

	void WidgetScene::Tick()
	{
		ImGui::Text(m_name.c_str());
	}
}
