#pragma once

namespace Editor
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
