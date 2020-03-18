#include "WidgetScene.h"

namespace Editor::Widget
{
	WidgetScene::WidgetScene() :
		IWidget("Scene")
	{
	}

	void WidgetScene::Tick()
	{
		ImGui::Text(m_nameID.c_str());
	}
}
