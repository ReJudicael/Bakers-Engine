#include "AWindow.h"
#include <iostream>

namespace Editor::Window
{
	AWindow::AWindow(const std::string& name, bool visible) :
		DrawableComponent{ name },
		isVisible{ visible }
	{
	}

	AWindow::~AWindow()
	{
		if (m_windowBegun)
			End();
	}

	bool AWindow::Begin()
	{
		if (!isVisible)
			return false;

		ImGui::Begin(GetNameID().c_str(), &isVisible, m_flags);
		m_windowBegun = true;
		return true;
	}

	void AWindow::End()
	{
		ImGui::End();
		m_windowBegun = false;
	}

	void AWindow::Focus()
	{
		if (isVisible)
			ImGui::SetWindowFocus(GetNameID().c_str());
	}

	void AWindow::Draw()
	{
		if (Begin())
		{
			Tick();
			End();
		}
	}
}
