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
		return true;
	}

	void IWidget::End()
	{
		ImGui::End();
	}

	void IWidget::Draw()
	{
		if (Begin())
		{
			Tick();
			End();
		}
	}
}
