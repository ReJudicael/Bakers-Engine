#include <imgui\imgui.h>
#include "Separator.h"

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
