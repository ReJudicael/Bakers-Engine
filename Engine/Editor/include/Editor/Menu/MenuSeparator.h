#pragma once

#include "AMenu.h"

namespace Editor::Menu
{
	/**
	 * Separator
	 */
	class MenuSeparator : public AMenu
	{
	public:
		/**
		 * Default constructor
		 */
		MenuSeparator();

		/**
		 * Default destructor
		 */
		~MenuSeparator() = default;

		/**
		 * Display Separator
		 */
		void Draw() override;
	};
}
