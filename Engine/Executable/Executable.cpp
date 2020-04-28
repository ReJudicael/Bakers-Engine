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
	rttr::library lib{ "Engine" };
	lib.load();

	if (!lib.is_loaded())
		std::cout << lib.get_error_string() << std::endl;

	Executable::ExeEngine* engine = new Executable::ExeEngine();
	int temp;
	if (temp = engine->Init())
		return temp;
	engine->MainLoop();
	delete engine;

	lib.unload();

	return 0;
}
