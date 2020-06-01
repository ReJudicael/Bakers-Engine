#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include <stdio.h>
#include <iostream>

#include "RootObject.hpp"
#include "InputSystem.hpp"
#include "ExeEngine.h"
#include "EngineCore.h"
#include "CoreMinimal.h"
#include <rttr/library.h>

int main()
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
		Executable::ExeEngine engine;
		int temp = engine.Init();
		if (temp)
			return temp;
		engine.MainLoop();
	}

	libEngine.unload();
	libGame.unload();

	return 0;
}
