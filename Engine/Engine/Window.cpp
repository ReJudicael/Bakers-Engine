#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"

Window::Window()
{
	Init(1280, 720);
}

Window::Window(const int height, const int width)
{
	Init(height, width);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::Init(const int height, const int width)
{
	if (!glfwInit())
		return;
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	m_window = glfwCreateWindow(height, width, "Default window", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		fprintf(stderr, "gladLoadGL failed. \n");
		return;
	}

	glfwSetWindowUserPointer(m_window, this);
	SetInputCallbackToGLFW();
}

void	Window::Update()
{
	Renderer r;
	Core::Datastructure::RootObject* o{ Core::Datastructure::RootObject::CreateRootNode() };
	Mesh* m{ r.CreatePlane() };
	o->AddComponent(m);
	r.AddMesh(m);

	Core::SystemManagement::InputSystem is(this);
	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		o->StartFrame();
		o->Update(0.2f);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		o->Render();

		if (is.IsKeyDown(EKey::S))
			std::cout << "Pressed" << std::endl;
		else if (is.IsKeyUp(EKey::S))
			std::cout << "Released" << std::endl;

		is.ClearRegisteredInputs();
	}
	o->Destroy();
	o->RemoveDestroyed();
}

void Window::SetInputCallbackToGLFW()
{
	GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressKey(static_cast<EKey>(key));
			else if (action == GLFW_RELEASE)
				this_window->OnReleaseKey(static_cast<EKey>(key));

			// std::cout << ToString((EKey)(key)) << std::endl;
		}
	};
	glfwSetKeyCallback(m_window, key_callback);

	GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressMouseButton(static_cast<EMouseButton>(button));
			else if (action == GLFW_RELEASE)
				this_window->OnReleaseMouseButton(static_cast<EMouseButton>(button));

			// std::cout << ToString((EMouseButton)(button)) << std::endl;
		}
	};
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}
