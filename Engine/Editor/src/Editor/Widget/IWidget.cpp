#include "IWidget.h"

namespace Editor::Widget
{
	IWidget::IWidget(const char* name) :
		m_name{ name }
	{
	}

	const char* IWidget::GetName() const
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
