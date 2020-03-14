#include "MenuSeparator.h"
#include <imgui\imgui.h>

Editor::Menu::MenuSeparator::MenuSeparator() :
	AMenu("MenuSeparator")
{
}

void Editor::Menu::MenuSeparator::Draw()
{
	ImGui::Separator();
}
