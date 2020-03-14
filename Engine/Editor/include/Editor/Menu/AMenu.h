#pragma once

#include "DrawableComponent.h"

namespace Editor
{
	/**
	 * Graphical control element which contains drop-down menus
	 */
	namespace Menu
	{
		/**
		 * Base class for MenuGroup and MenuItem
		 */
		class AMenu : public Datastructure::DrawableComponent
		{
		public:
			/**
			 * Specific constructor
			 * @param name: Name of menu
			 */
			AMenu(const std::string& name);

			/**
			 * Default destructor
			 */
			~AMenu() = default;
		};
	}
}
