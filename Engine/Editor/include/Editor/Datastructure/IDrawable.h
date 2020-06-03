#pragma once

#include "CoreMinimal.h"

namespace Editor::Datastructure
{
	/**
	 * Interface to draw UI class
	 */
	class IDrawable
	{
	public:
		/**
		 * Display window content
		 */
		virtual void Draw() = 0;

		/**
		 * Default destructor
		 */
		virtual ~IDrawable() = default;
	};
}
