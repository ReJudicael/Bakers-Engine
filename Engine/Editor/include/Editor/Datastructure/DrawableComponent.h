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
			 * Number of components (incremented when a component has been instantiated)
			 */
			static unsigned int _NB_COMPONENTS;

		protected:
			/**
			 * Name of component
			 */
			std::string m_name;

			/**
			 * ID of component ("##" + _NB_COMPONENTS)
			 */
			std::string m_ID;

		public:
			/**
			 * Explicit constructor
			 * @param name: Name of component
			 */
			DrawableComponent(const std::string& name = "");

			/**
			 * Default destructor
			 */
			~DrawableComponent() = default;

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
