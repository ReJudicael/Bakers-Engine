#include "CoreMinimal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <stb_image.h>
#include <rttr/library.h>
#include <stdio.h>
#include <iostream>

#include "GUIManager.h"
#include "RootObject.hpp"
#include "InputSystem.hpp"
#include "EditorEngine.h"
#include "CoreMinimal.h"

int main()
{
	{
		rttr::library libEngine{ "Engine" };
		rttr::library libGame{ "Game" };
		libEngine.load();
		if (!libEngine.is_loaded())
			std::cout << "Engine lib failed to load:" << std::endl << libEngine.get_error_string() << std::endl;
		libGame.load();
		if (!libGame.is_loaded())
			std::cout << "Game lib failed to load:" << std::endl << libEngine.get_error_string() << std::endl;
		{
			Editor::EditorEngine engine;
			int temp;
			if (temp = engine.Init())
				return temp;
			engine.MainLoop();
		}
#ifdef TRACY_ENABLE
		// Giving time for tracy to finish to track all memory allocations and zones
		std::cout << "Press enter to continue" << std::endl;
		std::cin.ignore();
#endif
		libEngine.unload();
		libGame.unload();
	}
	return 0;
}
