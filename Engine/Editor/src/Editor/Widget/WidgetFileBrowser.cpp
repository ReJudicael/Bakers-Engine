#include "WidgetFileBrowser.h"

namespace Editor::Widget
{
	WidgetFileBrowser::WidgetFileBrowser() :
		IWidget("File Browser")
	{
	}

	void WidgetFileBrowser::Tick()
	{
		ImGui::Text(m_name.c_str());
	}
}
