#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ExeEngine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Executable
{
	ExeEngine::ExeEngine() : ExeEngine(1280, 800)
	{
	}
	ExeEngine::ExeEngine(int width, int height) : EngineCore(width, height)
	{
	}
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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int display_w, display_h;
		glfwGetFramebufferSize(m_window, &display_w, &display_h);
		
		glViewport(0, 0, display_w, display_h);

		auto shader = m_manager->GetShader("TextureToQuad");
		shader->UseProgram();

		float f[3]{ 1.f, 1.f, 1.f };
		glUniform3fv(shader->GetLocation("uColor"), 1, f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(m_VAO); 
		Core::Renderer::Framebuffer* cam = GetFBO(0, Core::Renderer::FBOType::CAMERA);
		if (cam == nullptr)
			BAKERS_LOG_ERROR("No camera found");
		else
		{
			glBindTexture(GL_TEXTURE_2D, GetFBO(0, Core::Renderer::FBOType::CAMERA)->ColorTexture);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindTexture(GL_TEXTURE_2D, 0);
			if (cam->Size[2] != m_width || cam->Size[3] != m_height)
				cam->Resize(m_width, m_height);
		}
		glBindVertexArray(0);

		TRACY_GL_IMAGE_SEND(m_width, m_height)
			TracyGpuCollect
		glfwSwapBuffers(m_window);

		FrameMark
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
		int temp = EngineCore::OnInit(width, height);
		if (temp)
			return temp;

		if (!glfwInit())
			return 201;

		if (!gladLoadGL())
			return 202;

		{
			GLuint VBO;
			ZoneScopedN("Init rendering quad")
			{
				float Quad[24] =
				{
					 -1.f,-1.f ,  0.f, 0.f, // bl
					  1.f,-1.f ,  1.f, 0.f, // br
					  1.f, 1.f,  1.f, 1.f, // tr

					-1.f, 1.f ,  0.f, 1.f , // tl
					-1.f,-1.f,   0.f, 0.f, // bl
					 1.f, 1.f ,  1.f, 1.f, // tr
				};

				// Upload mesh to gpu
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), Quad, GL_STATIC_DRAW);
			}

			// Create a vertex array and bind it
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glBindVertexArray(0);

			m_manager->CreateShader("TextureToQuad", "Resources\\Shaders\\PostprocessShader.vert", "Resources\\Shaders\\PostprocessShader.frag");
		}

		glDebugMessageCallback(MessageCallback, 0);

		glfwMakeContextCurrent(m_window);
		{
			GLFWimage icons;
			icons.pixels = stbi_load("Resources\\Images\\bread.png", &icons.width, &icons.height, 0, STBI_rgb_alpha);
			glfwSetWindowIcon(m_window, 1, &icons);
			stbi_image_free(icons.pixels);
		}
		SetCallbackToGLFW();

		return 0;
	}

	Core::Maths::Vec2 ExeEngine::GetMousePos() noexcept
	{
		double pos[2];
		glfwGetCursorPos(GetWindow(), &pos[0], &pos[1]);
		return Core::Maths::Vec2(static_cast<float>(pos[0]), static_cast<float>(pos[1]));
	}

	Core::Maths::Vec2	ExeEngine::GetMousePosInWindow() noexcept
	{
		double pos[2];
		glfwGetCursorPos(GetWindow(), &pos[0], &pos[1]);
		int windowSize[2];
		glfwGetWindowSize(GetWindow(), &windowSize[0], &windowSize[1]);

		Core::Maths::Vec2 mouse;
		mouse.x = pos[0] - (float)windowSize[0] * 0.5f;
		mouse.x /= (float)windowSize[0] * 0.5f;
		mouse.y = pos[1] - (float)windowSize[1] * 0.5f;
		mouse.y /= (float)windowSize[1] * 0.5f;

		return mouse;
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
				this_window->OnResizeWindow(width, height);
			}
		};
		return glfwSetWindowSizeCallback(m_window, window_size_callback);
	}
}