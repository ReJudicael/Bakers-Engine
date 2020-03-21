#include "AWindow.h"
#include "Canvas.hpp"
#include <iostream>

namespace Editor::Window
{
	AWindow::AWindow(Canvas* canvas, const std::string& name, bool visible) :
		DrawableComponent{ name },
		m_canvas{ canvas },
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
		PushWindowStyle();
		if (Begin())
		{
			Tick();
			End();
		}
		PopWindowStyle();
	}

	Canvas* AWindow::GetCanvas() noexcept
	{
		return m_canvas;
	}

	EditorEngine* AWindow::GetEngine() noexcept
	{
		return m_canvas->GetEngine();
	}
}
