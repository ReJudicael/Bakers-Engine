#include "WidgetFileBrowser.h"

namespace Editor::Widget
{
	WidgetFileBrowser::WidgetFileBrowser() :
		IWidget(W_FILEBROWSER)
	{
	}

	void WidgetFileBrowser::Tick()
	{
		ImGui::Text(W_FILEBROWSER);
	}
}
