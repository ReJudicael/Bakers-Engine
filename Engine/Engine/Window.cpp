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
	SetCallbackToGLFW();
}

void Window::Update()
{
	Renderer r;
	Core::Datastructure::RootObject* o{ Core::Datastructure::RootObject::CreateRootNode() };
	Mesh* m{ r.CreatePlane() };
	o->AddComponent(m);
	r.AddMesh(m);

	m_inputSystem = new Core::SystemManagement::InputSystem(this);

	while (!glfwWindowShouldClose(m_window))
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		o->StartFrame();
		o->Update(0.2f);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		o->Render();

		// TODO: Delete Ln 66-107
		if (m_inputSystem->IsKeyPressed(EKey::W))
			std::cout << ToString(EKey::W) << " - " << ToString(m_inputSystem->GetKeyState(EKey::W)) << std::endl;
		//if (m_inputSystem->IsKeyDown(EKey::W))
		//	std::cout << ToString(EKey::W) << " - " << ToString(m_inputSystem->GetKeyState(EKey::W)) << std::endl;
		if (m_inputSystem->IsKeyUp(EKey::W))
			std::cout << ToString(EKey::W) << " - " << ToString(m_inputSystem->GetKeyState(EKey::W)) << std::endl;

		if (m_inputSystem->IsKeyPressed(EKey::S))
			std::cout << ToString(EKey::S) << " - " << ToString(m_inputSystem->GetKeyState(EKey::S)) << std::endl;
		//if (m_inputSystem->IsKeyDown(EKey::S))
		//	std::cout << ToString(EKey::S) << " - " << ToString(m_inputSystem->GetKeyState(EKey::S)) << std::endl;
		if (m_inputSystem->IsKeyUp(EKey::S))
			std::cout << ToString(EKey::S) << " - " << ToString(m_inputSystem->GetKeyState(EKey::S)) << std::endl;

		if (m_inputSystem->IsKeyPressed(EKey::A))
			std::cout << ToString(EKey::A) << " - " << ToString(m_inputSystem->GetKeyState(EKey::A)) << std::endl;
		//if (m_inputSystem->IsKeyDown(EKey::A))
		//	std::cout << ToString(EKey::A) << " - " << ToString(m_inputSystem->GetKeyState(EKey::A)) << std::endl;
		if (m_inputSystem->IsKeyUp(EKey::A))
			std::cout << ToString(EKey::A) << " - " << ToString(m_inputSystem->GetKeyState(EKey::A)) << std::endl;

		if (m_inputSystem->IsKeyPressed(EKey::D))
			std::cout << ToString(EKey::D) << " - " << ToString(m_inputSystem->GetKeyState(EKey::D)) << std::endl;
		//if (m_inputSystem->IsKeyDown(EKey::D))
		//	std::cout << ToString(EKey::D) << " - " << ToString(m_inputSystem->GetKeyState(EKey::D)) << std::endl;
		if (m_inputSystem->IsKeyUp(EKey::D))
			std::cout << ToString(EKey::D) << " - " << ToString(m_inputSystem->GetKeyState(EKey::D)) << std::endl;

		if (m_inputSystem->IsKeyPressed(EKey::SPACE))
			std::cout << ToString(EKey::SPACE) << " - " << ToString(m_inputSystem->GetKeyState(EKey::SPACE)) << std::endl;
		if (m_inputSystem->IsKeyDown(EKey::SPACE))
			std::cout << ToString(EKey::SPACE) << " - " << ToString(m_inputSystem->GetKeyState(EKey::SPACE)) << std::endl;
		if (m_inputSystem->IsKeyUp(EKey::SPACE))
			std::cout << ToString(EKey::SPACE) << " - " << ToString(m_inputSystem->GetKeyState(EKey::SPACE)) << std::endl;

		if (m_inputSystem->IsMouseButtonPressed(EMouseButton::LEFT))
			std::cout << ToString(EMouseButton::LEFT) << " - " << ToString(m_inputSystem->GetMouseButtonState(EMouseButton::LEFT)) << std::endl;
		if (m_inputSystem->IsMouseButtonDown(EMouseButton::LEFT))
			std::cout << ToString(EMouseButton::LEFT) << " - " << ToString(m_inputSystem->GetMouseButtonState(EMouseButton::LEFT)) << std::endl;
		if (m_inputSystem->IsMouseButtonUp(EMouseButton::LEFT))
			std::cout << ToString(EMouseButton::LEFT) << " - " << ToString(m_inputSystem->GetMouseButtonState(EMouseButton::LEFT)) << std::endl;

		static int zoom = 100;
		if (m_inputSystem->IsKeyDown(EKey::LEFT_CONTROL))
		{
			if (m_inputSystem->IsScrollUp())
			{
				zoom += 10;
				std::cout << zoom << std::endl;
			}
			else if (m_inputSystem->IsScrollDown())
			{
				zoom -= 10;
				std::cout << zoom << std::endl;
			}
		}

		if (m_inputSystem->IsMouseButtonUp(EMouseButton::LEFT))
			std::cout << ToString(m_inputSystem->GetMouseButtonState(EMouseButton::LEFT)) << std::endl;
		if (m_inputSystem->IsMouseDoubleClicked(EMouseButton::MIDDLE))
			std::cout << "Middle\n";

		//std::cout << ToString(m_inputSystem->GetMouseButtonState(EMouseButton::LEFT)) << std::endl;

		m_inputSystem->ClearRegisteredInputs();
	}

	delete m_inputSystem;

	o->Destroy();
	o->RemoveDestroyed();
}

void Window::SetCallbackToGLFW()
{
	SetKeyCallBackToGLFW();
	SetMouseButtonCallBackToGLFW();
	SetScrollCallBackToGLFW();
}

GLFWkeyfun Window::SetKeyCallBackToGLFW()
{
	GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressKey(static_cast<EKey>(key));
			if (action == GLFW_RELEASE)
				this_window->OnReleaseKey(static_cast<EKey>(key));
		}
	};
	return glfwSetKeyCallback(m_window, key_callback);
}

GLFWmousebuttonfun Window::SetMouseButtonCallBackToGLFW()
{
	GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			if (action == GLFW_PRESS)
				this_window->OnPressMouseButton(static_cast<EMouseButton>(button));
			else if (action == GLFW_RELEASE)
				this_window->OnReleaseMouseButton(static_cast<EMouseButton>(button));
		}
	};
	return glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

GLFWscrollfun Window::SetScrollCallBackToGLFW()
{
	GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			this_window->OnScrollYAxis(yoffset);
		}
	};
	return glfwSetScrollCallback(m_window, scroll_callback);
}
