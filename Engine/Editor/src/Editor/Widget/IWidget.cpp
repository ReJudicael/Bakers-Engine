#include "IWidget.h"

namespace Editor::Widget
{
	IWidget::IWidget(const char* name) :
		m_name{ name }
	{
	}

	bool IWidget::Begin()
	{
		if (!m_isVisible)
			return false;

		ImGui::Begin(m_name, &m_isVisible, m_flags);
		m_windowBegun = true;
		return true;
	}

	bool IWidget::End()
	{
		if (!m_windowBegun)
			return false;

		ImGui::End();
		m_windowBegun = false;
		return true;
	}

	void IWidget::Draw()
	{
		Begin();
		Tick();
		End();
	}
}
