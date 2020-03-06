#include "WidgetFileBrowser.h"

namespace Editor::Widget
{
	WidgetFileBrowser::WidgetFileBrowser(const char* name) :
		IWidget(name)
	{
	}

	void WidgetFileBrowser::Tick()
	{
		ImGui::Text(m_name);
	}
}
