#pragma once

#include "IDrawable.h"
#include <string>

namespace Editor
{
	/**
	 * Data organization that enables efficient access and modification for UI classes
	 */
	namespace Datastructure
	{
		/**
		 * Component drawable of a canvas
		 */
		class DrawableComponent : public virtual IDrawable
		{
		private:
			/**
			 * ID of component (incremented when a component has been instantiated)
			 */
			static unsigned int _ID_COMPONENT_INCREMENT;

		protected:
			/**
			 * Name of component
			 */
			std::string m_name;

			/**
			 * ID of component (##0)
			 */
			std::string m_ID;

		public:
			/**
			 * Explicite constructor
			 * @param name: Name of component
			 */
			DrawableComponent(const std::string& name = "");

			/**
			 * Get ID of component
			 * @return ID of component
			 */
			const std::string& GetID() const;

			/**
			 * Get name of component
			 * @return Name of component
			 */
			const std::string& GetName() const;

			/**
			 * Get name ID of component
			 * @return Name ID of component
			 */
			std::string GetNameID() const;
		};
	}
}
