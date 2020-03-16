#include "Separator.h"
#include <imgui\imgui.h>

namespace Editor::Widget
{
	Separator::Separator() :
		AWidget("Separator")
	{
	}

	void Editor::Widget::Separator::Draw()
	{
		ImGui::Separator();
	}
}
