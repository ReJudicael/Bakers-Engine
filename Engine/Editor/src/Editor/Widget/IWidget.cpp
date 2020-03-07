#include "IWidget.h"
#include <iostream>

namespace Editor::Widget
{
	unsigned int IWidget::_ID_WIDGET_INCREMENT = 0;

	IWidget::IWidget(std::string name) :
		m_name{ name }
	{
		m_name += "##" + std::to_string(_ID_WIDGET_INCREMENT++);
	}

	std::string IWidget::GetName() const
	{
		return m_name;
	}

	bool IWidget::IsVisible() const
	{
		return m_isVisible;
	}

	bool IWidget::Begin()
	{
		if (!m_isVisible)
			return false;

		ImGui::Begin(m_name.c_str(), &m_isVisible, m_flags);
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
