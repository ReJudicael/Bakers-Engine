#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "GUIManager.h"
#include "stb_image.h"
#include <stdio.h>
#include <iostream>

#include "RootObject.hpp"
#include "InputSystem.hpp"
#include "EditorEngine.h"
#include "CoreMinimal.h"
#include <rttr/library.h>

int main()
{
	rttr::library lib{ "Engine" };
	lib.load();

	if (!lib.is_loaded())
		std::cout << lib.get_error_string() << std::endl;

	Editor::EditorEngine* engine = new Editor::EditorEngine();
	int temp;
	if (temp = engine->Init())
		return temp;
	engine->MainLoop();
	delete engine;

	lib.unload();

	return 0;
}
