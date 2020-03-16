#pragma once

#include "AWidget.h"

namespace Editor::Widget
{
	/**
	 * Separator
	 */
	class Separator : public AWidget
	{
	public:
		/**
		 * Default constructor
		 */
		Separator();

		/**
		 * Default destructor
		 */
		~Separator() = default;

		/**
		 * Display Separator
		 */
		void Draw() override;
	};
}
