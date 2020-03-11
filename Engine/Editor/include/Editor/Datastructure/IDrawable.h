#pragma once

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
	};
}
