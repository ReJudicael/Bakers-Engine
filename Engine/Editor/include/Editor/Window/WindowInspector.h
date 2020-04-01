#pragma once

#include "AWindow.h"
#include "ComponentBase.h"
#include "CoreMinimal.h"

namespace Core::Datastructure
{
	class Object;
}

namespace Editor::Window
{
	/**
	 * Window Inspector
	 */
	class WindowInspector final : public AWindow
	{
	private:
		/**
		 * Flags for Collapsing Header
		 */
		ImGuiTreeNodeFlags m_treeNodeFlags;

	private:
		/**
		 * Buffer of InputText (to rename the objectSelected)
		 */
		char m_name[64]{};

		/**
		 * Whether the user wants to modify the local transform or not
		 */
		bool m_isLocal{ true };

	public:
		/**
		 * Constructor which set title of window ("Inspector")
		 */
		WindowInspector(Canvas* canvas, bool visible = true);

		/**
		 * Default destructor
		 */
		~WindowInspector() = default;

	private:
		/**
		 * Push window style
		 */
		void PushWindowStyle() override;

		/**
		 * Pop window style
		 */
		void PopWindowStyle() override;

	private:
		/**
		 * Display name in hierarchy
		 * @param object: Selected object
		 */
		void DisplayObjectName(Core::Datastructure::Object* object);

		/**
		 * Display local transform of object in hierarchy
		 * @param object: Selected object
		 */
		void DisplayObjectLocalTransform(Core::Datastructure::Object* object);

		/**
		 * Display global transform object in hierarchy
		 * @param object: Selected object
		 */
		void DisplayObjectGlobalTransform(Core::Datastructure::Object* object);

		/**
		 * Display transform of object in hierarchy
		 * @param object: Selected object
		 */
		void DisplayObjectTransform(Core::Datastructure::Object* object);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;

		/**
		 * Draw a property in window
		 * @param prop: Property to draw
		 */
		void DrawProperty(rttr::property prop, Core::Datastructure::ComponentBase* component);
		/**
		 * Draw an enum property in window
		 * @param prop: Property to draw
		 */
		void DrawEnum(rttr::property prop, Core::Datastructure::ComponentBase* component);
	};
}
