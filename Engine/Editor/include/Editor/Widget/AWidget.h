#pragma once

#include "DrawableUIComponent.h"

namespace Editor
{
	/**
	 * A control element in a graphical user interface - an element of interaction
	 */
	namespace Widget
	{
		/**
		 * Base class for widgets
		 */
		class AWidget : public Datastructure::DrawableUIComponent
		{
		public:
			/**
			 * Specific constructor
			 * @param name: Name of menu
			 */
			AWidget(const std::string& name);

			/**
			 * Default destructor
			 */
			virtual ~AWidget() = default;
		};
	}
}
