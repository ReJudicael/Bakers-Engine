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

		/**
		 * Draw an enum property in window
		 * @param prop: Property to draw
		 * @param component:
		 */
		void DrawEnum(rttr::property prop, Core::Datastructure::ComponentBase* component);

		/**
		 * Draw a property of the component in window
		 * @param prop: Property to draw
		 * @param component:
		 */
		void DrawProperty(rttr::property prop, Core::Datastructure::ComponentBase* component);

		/**
		 * Display the components of the object
		 * @param object: Chosen object
		 */
		void DisplayObjectComponents(Core::Datastructure::Object* object);

		/**
		 * Add component to the object
		 * @param object: Chosen object
		 */
		void AddComponentToObjectButton(Core::Datastructure::Object* object);

		/**
		 * Display the names and current values of the properties
		 * The user can be modify these values
		 * @param object: Chosen object
		 */
		void ObjectInspector(Core::Datastructure::Object* object);

	private:
		/**
		 * Draw elements in window
		 */
		void Tick() override;
	};
}
