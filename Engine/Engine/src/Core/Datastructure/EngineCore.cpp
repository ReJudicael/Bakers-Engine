#include "EngineCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RootObject.hpp"
#include "LoadResources.h"
#include "PlayerCamera.h"

RTTR_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<EngineCore>("IWindow")
		.method("Update", &EngineCore::Update)
		.method("Init", select_overload<int(void)>(&EngineCore::Init))
		.method("Init", select_overload<int(int, int)>(&EngineCore::Init))
		.method("SetSizeWindow", &EngineCore::SetSizeWindow)
		.property_readonly("DeltaTime", &EngineCore::GetDeltaTime);
}

namespace Core::Datastructure
{
	double EngineCore::GetDeltaTime()
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - m_time;

		m_time = currentTime;

		return deltaTime;
	}

	EngineCore::EngineCore() : EngineCore(1280, 800)
	{}

	EngineCore::EngineCore(const int width, const int height) : m_width{ width }, m_height{ height }
	{
		m_inputSystem = new Core::SystemManagement::InputSystem(this);
		m_root = Core::Datastructure::RootObject::CreateRootNode(m_inputSystem);
	}

	EngineCore::~EngineCore()
	{
		delete m_root;
		delete m_inputSystem;
	}

	int EngineCore::Init()
	{
		return Init(m_width, m_height);
	}

	void	error(int error, const char* message)
	{
		std::cout << message << std::endl;
	}

	int EngineCore::Init(int width, int height)
	{
		glfwSetErrorCallback(error);

		if (!glfwInit())
			return 1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

		m_window = glfwCreateWindow(width, height, "Editor", nullptr, nullptr);
		if (m_window == nullptr)
			return 1;

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1);

		if (!gladLoadGL())
		{
			fprintf(stderr, "gladLoadGL failed. \n");
			return 1;
		}
		
		TracyGpuContext

		m_fbo = new Core::Renderer::Framebuffer(width, height);
		return 0;
	}

	void	EngineCore::Update()
	{
		ZoneNamedN(updateLoop, "Main update loop", true)
		m_root->StartFrame();
		double deltaTime = GetDeltaTime();
		m_root->Update(deltaTime);
		GLint PreviousFramebuffer;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
		glClear(GL_COLOR_BUFFER_BIT);
		m_root->Render();
		TRACY_GL_IMAGE_SEND(m_width, m_height)
		TracyGpuCollect
		glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);
		FrameMark;
		m_root->RemoveDestroyed();
		m_inputSystem->ClearRegisteredInputs();
	}
	Core::Renderer::Framebuffer* EngineCore::GetFBO()
	{
		return m_fbo;
	}

	GLFWwindow* EngineCore::GetWindow()
	{
		return m_window;
	}
}