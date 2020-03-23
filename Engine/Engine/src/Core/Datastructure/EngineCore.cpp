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

	EngineCore::EngineCore(const int width, const int height) : m_width{ width }, m_height{ height }, m_fbo{ nullptr }, m_window{ nullptr }
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
		glDebugMessageCallback(MessageCallback, 0);

		TracyGpuContext

			m_fbo = new Core::Renderer::Framebuffer(width, height);
		m_manager = new Resources::Loader::ResourcesManager();

		Core::Datastructure::Object* camNode{ m_root->CreateChild("Camera", {}) };

		PlayerCamera* c = new PlayerCamera(1200.f / 700.f, 60, 0.1, 100);
		camNode->AddComponent(c);

		Core::Datastructure::Object* o{ m_root->CreateChild("Scene", {}) };

		//manager.LoadResourcesIRenderable("Resources/Umbreon/UmbreonHighPoly.obj", o);
		m_manager->LoadResourcesIRenderable("Resources/level.fbx", o);

		o->SetScale({ 0.1,0.1,0.1 });
		return 0;
	}

	void	EngineCore::Update()
	{
		ZoneNamedN(updateLoop, "Main update loop", true)
			double deltaTime = GetDeltaTime();

		StartFrame();

		m_root->Update(deltaTime);
		
		Render();

		EndFrame();
	}
	Core::Renderer::Framebuffer* EngineCore::GetFBO()
	{
		return m_fbo;
	}

	GLFWwindow* EngineCore::GetWindow()
	{
		return m_window;
	}

	void		EngineCore::StartFrame()
	{
		ZoneScoped
			m_root->StartFrame();
			m_manager->LinkAllTextureToOpenGl();
			m_manager->LinkAllModelToOpenGl();
	}

	void		EngineCore::Render()
	{
		ZoneScoped
			GLint PreviousFramebuffer;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->FBO);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_root->Render();
			TRACY_GL_IMAGE_SEND(m_width, m_height)
			TracyGpuCollect
			glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);
		FrameMark
	}
	
	void		EngineCore::EndFrame()
	{
		m_root->RemoveDestroyed();
		m_inputSystem->ClearRegisteredInputs();
	}
}