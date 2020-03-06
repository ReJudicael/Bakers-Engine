#include "WidgetScene.h"

namespace Editor::Widget
{
	WidgetScene::WidgetScene(const char* name) :
		IWidget(name)
	{
	}

	void WidgetScene::Tick()
	{
		ImGui::Text(m_name);
	}
}
