#include "AWindow.h"
#include <iostream>

namespace Editor::Window
{
	AWindow::AWindow(const std::string& name) :
		DrawableComponent{ name }
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

		m_windowBegun = ImGui::Begin(GetNameID().c_str(), &isVisible, m_flags);
		return m_windowBegun;
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
