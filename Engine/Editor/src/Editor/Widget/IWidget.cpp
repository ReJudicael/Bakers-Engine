#include "IWidget.h"
#include <iostream>

namespace Editor::Widget
{
	unsigned int IWidget::_ID_WIDGET_INCREMENT = 0;

	IWidget::IWidget(std::string name) :
		m_name{ name }
	{
		m_ID = "##" + std::to_string(_ID_WIDGET_INCREMENT++);
		m_nameID = m_name + m_ID;
	}

	IWidget::~IWidget()
	{
		if (m_windowBegun)
			End();
	}

	std::string IWidget::GetNameID() const
	{
		return m_nameID;
	}

	std::string IWidget::GetName() const
	{
		return m_name;
	}

	std::string IWidget::GetID() const
	{
		return m_ID;
	}

	bool IWidget::Begin()
	{
		if (!isVisible)
			return false;

		ImGui::Begin(m_nameID.c_str(), &isVisible, m_flags);
		m_windowBegun = true;
		return true;
	}

	void IWidget::End()
	{
		ImGui::End();
		m_windowBegun = false;
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
