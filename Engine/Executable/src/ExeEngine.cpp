#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ExeEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Executable
{
	ExeEngine::~ExeEngine()
	{

	}

	void ExeEngine::MainLoop()
	{
		if (m_state == Core::Datastructure::EngineState::INITIALIZED)
			m_state = Core::Datastructure::EngineState::RUNNING;
		while (!glfwWindowShouldClose(m_window) && m_state != Core::Datastructure::EngineState::CLOSED)
		{
			OnLoop();
		}
	}

	void ExeEngine::OnLoop()
	{
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		EngineCore::OnLoop();

		int display_w, display_h;
		glfwGetFramebufferSize(m_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glfwSwapBuffers(m_window);
	}

	void ExeEngine::SetSizeWindow(const int width, const int height)
	{
		m_width = width;
		m_height = height;
	}

	void callback_error(int error, const char* message)
	{
		std::cerr << message << std::endl;
	}

	void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	int ExeEngine::OnInit(const int width, const int height)
	{
		glfwSetErrorCallback(callback_error);
		int temp;
		if (temp = EngineCore::OnInit(width, height))
			return temp;

		if (!glfwInit())
			return 201;

		if (!gladLoadGL())
			return 202;

		glDebugMessageCallback(MessageCallback, 0);

		glfwMakeContextCurrent(m_window);
		{
			GLFWimage icons;
			icons.pixels = stbi_load("Resources\\Images\\bread.png", &icons.width, &icons.height, 0, STBI_rgb_alpha);
			glfwSetWindowIcon(m_window, 1, &icons);
			stbi_image_free(icons.pixels);
		}
		SetCallbackToGLFW();

		INIT_TRACY_GL_IMAGE(320, 180)

		return 0;
	}

	Core::Maths::Vec2 ExeEngine::GetMousePos() noexcept
	{
		double pos[2];
		glfwGetCursorPos(GetWindow(), &pos[0], &pos[1]);
		return Core::Maths::Vec2(static_cast<float>(pos[0]), static_cast<float>(pos[1]));
	}


	void ExeEngine::SetCallbackToGLFW()
	{
		OnResizeWindow += BIND_EVENT_2(ExeEngine::SetSizeWindow);

		SetKeyCallBackToGLFW();
		SetMouseButtonCallBackToGLFW();
		SetScrollCallBackToGLFW();
		SetWindowSizeToGLFW();
	}

	GLFWkeyfun ExeEngine::SetKeyCallBackToGLFW()
	{
		GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ZoneScopedN("InputSystemKeyUpdate")
				ZoneText("Updating key presses", 21)
				ExeEngine* this_window = reinterpret_cast<ExeEngine*>(glfwGetWindowUserPointer(window));
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

	GLFWmousebuttonfun ExeEngine::SetMouseButtonCallBackToGLFW()
	{
		GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			ZoneScopedN("InputSystemMouseButtonUpdate")
				ZoneText("Updating MouseButton presses", 29)
				ExeEngine* this_window = reinterpret_cast<ExeEngine*>(glfwGetWindowUserPointer(window));
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

	GLFWscrollfun ExeEngine::SetScrollCallBackToGLFW()
	{
		GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			ZoneScopedN("InputSystemScrollUpdate")
				ZoneText("Updating Scrolling", 19)
				ExeEngine* this_window = reinterpret_cast<ExeEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnScrollYAxis(yoffset);
			}
		};
		return glfwSetScrollCallback(m_window, scroll_callback);
	}

	GLFWwindowsizefun ExeEngine::SetWindowSizeToGLFW()
	{
		GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
		{
			ExeEngine* this_window = reinterpret_cast<ExeEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				//this_window->OnResizeWindow(width, height);
			}
		};
		return glfwSetWindowSizeCallback(m_window, window_size_callback);
	}
}