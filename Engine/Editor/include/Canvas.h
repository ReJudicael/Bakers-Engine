#pragma once

#include "IRenderable.h"
#include <imgui\imgui.h>
#include <imgui\imgui_internal.h>
#include <vector>

#define W_SCENE "Scene"
#define W_HIERARCHY "Hierarchy"
#define W_INSPECTOR "Inspector"
#define W_FILEBROWSER "File Browser"
#define W_CONSOLE "Console"

class Widget;
class Canvas : public IRenderable
{
private:
	std::vector<Widget> m_widgets;

public:
	void AddWidget(const Widget& panel);
	void RemoveWidget(const Widget& panel);
	void RemoveAllPanels();
	void SetDockspace();

	void Draw() override;
};

