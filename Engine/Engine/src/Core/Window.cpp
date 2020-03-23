//#include <glad/glad.h>
#include <stdio.h>

#include "Window.h"
#include "Renderer.h"
#include "RootObject.hpp"
#include "ComponentUpdatable.h"
#include "Loadresources.h"
#include "Debug.h"
#include "TaskSystem.hpp"
#include "PlayerCamera.h"
#include "Maths.hpp"
#include "Shader.h"

Window::Window():
	m_width{ 1280 },
	m_height{ 800 }
{
	Init(m_width, m_height);
}

Window::Window(const int width, const int height) :
	m_width{ width },
	m_height{ height }
{
	Init(m_width, m_height);
}

void Window::Init(const int width, const int height)
{
	ZoneScoped
		ZoneText("Initializing window", 20)
	if (!glfwInit())
		return;

	glfwDefaultWindowHints();
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, "Default window", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		fprintf(stderr, "gladLoadGL failed. \n");
		return;
	}
	TracyGpuContext

	OnResizeWindow += BIND_EVENT_2(Window::SetSizeWindow);

	glfwSetWindowUserPointer(m_window, this);
	SetCallbackToGLFW();

	m_inputSystem = new Core::SystemManagement::InputSystem(this);
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();

	if (m_inputSystem)
		delete m_inputSystem;
}

void	Window::Update()
{
	Resources::Loader::ResourcesManager manager{};
	{
		ZoneScopedN("Engine init scene")
			ZoneText("Initializing scene of the engine", 33)
		m_root = Core::Datastructure::RootObject::CreateRootNode(m_inputSystem);
		Core::Datastructure::Object* camNode{ m_root->CreateChild({}) };

		PlayerCamera* c = new PlayerCamera(1200.f / 700.f, 60, 0.1, 100);
		camNode->AddComponent(c);
		Renderer r;

		NRenderer::Light* l = new NRenderer::Light();
		l->SetLightType(NRenderer::Light::ELightType::POINT);
		l->SetAmbiant({ 0.1f, 0.1f, 0.1f });
		l->SetDiffuse({ 0.1f, 0.2f, 0.2f });
		l->SetSpecular({ 0.1f, 0.2f, 0.2f });
		l->SetAttenuation({ 1.f, 0.022f, 0.0019f });
		l->SetRange(100.f);
		l->SetAngle(0.5f);
		l->SetAngleSmoothness(0.01f);
		camNode->AddComponent(l);

		Core::Datastructure::Object* o{ m_root->CreateChild({}) };

		//manager.Load3DObject("Resources/Umbreon/UmbreonHighPoly.obj");
		//manager.Load3DObject("Resources/Dog/12228_Dog_v1_L2.obj");
		manager.Load3DObject("Resources/level.fbx");

		Resources::Object3DGraph::CreateScene("Resources/level.fbx", manager, o);
		//Resources::Object3DGraph::CreateScene("Resources/Umbreon/UmbreonHighPoly.obj", manager, o);
		//Resources::Object3DGraph::CreateScene("Resources/Dog/12228_Dog_v1_L2.obj", manager, o);

		o->SetScale({ 0.1,0.1,0.1 });
	}
	INIT_TRACY_GL_IMAGE(320, 180)

	while (!glfwWindowShouldClose(m_window))
	{
		ZoneNamedN(updateLoop, "Main update loop", true)
		glfwPollEvents();
		manager.LinkAllTextureToOpenGl();
		manager.LinkAllModelToOpenGl();
		m_root->StartFrame();
		double deltaTime = GetDeltaTime();
		m_root->Update(deltaTime);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_root->Render();
		{
			ZoneNamedN(swapBuffers, "glfwSwapBuffers", true)
			TRACY_GL_IMAGE_SEND(m_width, m_height)
			glfwSwapBuffers(m_window);
			TracyGpuCollect
		}
		FrameMark;
		m_root->RemoveDestroyed();
		m_inputSystem->ClearRegisteredInputs();
	}
	{
		ZoneScopedN("Cleanup scene")
			ZoneText("Delete scene and clean up conponents and objects", 49)
		m_root->Destroy();
		delete m_root;
	}
}

double Window::GetDeltaTime()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - m_time;

	m_time = currentTime;

	return deltaTime;
}

void Window::SetSizeWindow(const double width, const double height)
{
	m_width = width;
	m_height = height;
	if (m_height != 0)
		m_root->SetCamerasRatio(static_cast<float>(m_width / m_height));
}

void Window::SetCallbackToGLFW()
{
	SetKeyCallBackToGLFW();
	SetMouseButtonCallBackToGLFW();
	SetScrollCallBackToGLFW();
	SetWindowSizeToGLFW();
}

GLFWkeyfun Window::SetKeyCallBackToGLFW()
{
	GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		ZoneScopedN("InputSystemKeyUpdate")
			ZoneText("Updating key presses", 21)
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
		ZoneScopedN("InputSystemMouseButtonUpdate")
			ZoneText("Updating MouseButton presses", 29)
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
		ZoneScopedN("InputSystemScrollUpdate")
			ZoneText("Updating Scrolling", 19)
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			this_window->OnScrollYAxis(yoffset);
		}
	};
	return glfwSetScrollCallback(m_window, scroll_callback);
}

GLFWwindowsizefun Window::SetWindowSizeToGLFW()
{
	GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
	{
		Window* this_window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
		if (this_window)
		{
			this_window->OnResizeWindow(width, height);
		}
	};
	return glfwSetWindowSizeCallback(m_window, window_size_callback);
}
