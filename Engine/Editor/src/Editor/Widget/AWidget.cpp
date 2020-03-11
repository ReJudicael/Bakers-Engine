#include "AWidget.h"
#include <iostream>

namespace Editor::Widget
{
	AWidget::AWidget(const std::string& name) :
		DrawableComponent{ name }
	{
	}

	AWidget::~AWidget()
	{
		if (m_windowBegun)
			End();
	}

	bool AWidget::Begin()
	{
		if (!isVisible)
			return false;

		ImGui::Begin(GetNameID().c_str(), &isVisible, m_flags);
		m_windowBegun = true;
		return true;
	}

	void AWidget::End()
	{
		ImGui::End();
		m_windowBegun = false;
	}

	void AWidget::Focus()
	{
		if (isVisible)
			ImGui::SetWindowFocus(GetNameID().c_str());
	}

	void AWidget::Draw()
	{
		if (Begin())
		{
			Tick();
			End();
		}
	}
}
