#include "CoreMinimal.h"

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

#include <iostream>

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
		//std::cin.ignore();
		libEngine.unload();
		libGame.unload();
	}
	return 0;
}
