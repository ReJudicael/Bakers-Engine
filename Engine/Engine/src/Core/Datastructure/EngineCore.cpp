#include "EngineCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RootObject.hpp"
#include "LoadResources.h"
#include "PlayerCamera.h"
#include "ScriptedComponent.h"
#include "Framebuffer.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "StaticMesh.h"
#include "Physics/PhysicsScene.h"
#include "DynamicMesh.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace Core::Datastructure;
	registration::class_<EngineCore>("EngineCore")
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

	EngineCore::EngineCore(const int width, const int height) : m_width{ width }, m_height{ height }, m_fbo{ nullptr }, m_window{ nullptr }, m_manager {nullptr}
	{
		m_inputSystem = new Core::SystemManagement::InputSystem(this);
		m_root = Core::Datastructure::RootObject::CreateRootNode(m_inputSystem, this);
	}

	EngineCore::~EngineCore()
	{
		delete m_root;
		delete m_inputSystem;
		for (auto fbo : m_fbo)
		{
			delete fbo;
		}
		delete m_manager;
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		m_window = glfwCreateWindow(width, height, "Bakers Engine", nullptr, nullptr);
		if (m_window == nullptr)
			return 2;

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSwapInterval(1);
		SetCallbackToGLFW();

		if (!gladLoadGL())
		{
			fprintf(stderr, "gladLoadGL failed. \n");
			return 3;
		}
		glDebugMessageCallback(MessageCallback, 0);

		TracyGpuContext

		m_physicsScene = new Core::Physics::PhysicsScene();

		if (!m_physicsScene->InitPhysX())
			return -1;

		m_manager = new Resources::Loader::ResourcesManager();

		Core::Datastructure::Object* camNode{ m_root->CreateChild("Camera", {}) };
		camNode->SetPos({ 1.f, 3.f, 0.f });

		PlayerCamera* c = new PlayerCamera(1200.f / 700.f, 60, 0.1, 100);
		camNode->AddComponent(c);

		NRenderer::Light* l = new NRenderer::Light();
		l->SetLightType(NRenderer::Light::ELightType::POINT);
		l->SetAmbiant({ 0.3f, 0.3f, 0.3f });
		l->SetDiffuse({ 0.2f, 0.2f, 0.2f });
		l->SetSpecular({ 0.2f, 0.2f, 0.2f });
		l->SetAttenuation({ 1.f, 0.022f, 0.0019f });
		l->SetRange(1000.f);
		l->SetAngle(0.5f);
		l->SetAngleSmoothness(0.01f);
		camNode->AddComponent(l);

		Core::Datastructure::ScriptedComponent* s = new Core::Datastructure::ScriptedComponent("./Resources/Scripts/DefaultScript.lua");
		camNode->AddComponent(s);
		Core::Datastructure::Object* o{ m_root->CreateChild("Scene", {}) };
		Core::Datastructure::Object* o2{ o->CreateChild("Scene2", {}) };
		Core::Datastructure::Object* obj{ m_root->CreateChild("Scene3", {}) };

		Core::Datastructure::Object* scene{ m_root->CreateChild("Scene", {}) };
		//m_manager->Load3DObject("Resources/level.fbx");
		//Resources::Object3DGraph::CreateScene("Resources/level.fbx", *m_manager, scene);
		obj->AddComponent(new Core::Physics::DynamicMesh());

		scene->SetScale({ 0.01f, 0.01f, 0.01f });
		o2->SetPos({ 0.f,-2.f,0.f });
		o2->SetScale({ 500.f,1.f,500.f });
		Core::Physics::StaticMesh* staticmesh1 = new Core::Physics::StaticMesh();
		((Core::Physics::BoxCollider*)staticmesh1->GetCollider())->SetBoxHalfExtent({ 500.f,1.f,500.f });
		o2->AddComponent(staticmesh1);

		m_manager->Load3DObject("Resources/Umbreon/UmbreonHighPoly.obj");
		//m_manager->Load3DObject("Resources/level.fbx");
		Resources::Object3DGraph::CreateScene("Resources/Umbreon/UmbreonHighPoly.obj", *m_manager, obj);
		//Resources::Object3DGraph::CreateScene("Resources/level.fbx", *m_manager, o);



		obj->SetPos({ 0.f,10.f,-10.f });
		o->SetScale({ 0.1,0.1,0.1 });
		m_fbo.clear();
		return 0;
	}

	void	EngineCore::DoLoop()
	{
		ZoneNamedN(updateLoop, "Main loop iteration", true)
		
		double deltaTime = GetDeltaTime();

		StartFrame();

		Update(deltaTime);

		Render();

		EndFrame();
	}
	void	EngineCore::Update(double deltaTime)
	{
		m_physicsScene->BeginSimulate(deltaTime);
		m_physicsScene->EndSimulate();
		m_root->Update(deltaTime);
	}

	Core::SystemManagement::InputSystem* EngineCore::GetInputSystem()
	{
		return m_inputSystem;
	}

	Core::Renderer::Framebuffer* EngineCore::GetFBO(int num)
	{
		auto it{ m_fbo.begin() };
		std::advance(it, num);

		return *it;
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
			m_manager->ShaderUpdate();
	}

	void		EngineCore::Render()
	{
		ZoneScoped
			m_root->Render(m_fbo);

		TRACY_GL_IMAGE_SEND(m_width, m_height)
			TracyGpuCollect

		FrameMark
	}
	
	void		EngineCore::EndFrame()
	{
		m_root->RemoveDestroyed();
		
		m_inputSystem->ClearRegisteredInputs();
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO()
	{
		return CreateFBO(m_width, m_height);
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO(int width, int height)
	{
		return m_fbo.emplace_back(new Core::Renderer::Framebuffer(width, height));
	}

	void EngineCore::DeleteFBO(Core::Renderer::Framebuffer* fbo)
	{
		for (auto it{ m_fbo.begin() }; it != m_fbo.end(); ++it)
		{
			if (*it == fbo)
			{
				it = m_fbo.erase(it);
				delete fbo;
			}
		}
	}


	void EngineCore::SetCallbackToGLFW()
	{
		SetKeyCallBackToGLFW();
		SetMouseButtonCallBackToGLFW();
		SetScrollCallBackToGLFW();
		SetWindowSizeToGLFW();
	}

	GLFWkeyfun EngineCore::SetKeyCallBackToGLFW()
	{
		GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ZoneScopedN("InputSystemKeyUpdate")
				ZoneText("Updating key presses", 21)
				EngineCore* this_window = reinterpret_cast<EngineCore*>(glfwGetWindowUserPointer(window));
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

	GLFWmousebuttonfun EngineCore::SetMouseButtonCallBackToGLFW()
	{
		GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			ZoneScopedN("InputSystemMouseButtonUpdate")
				ZoneText("Updating MouseButton presses", 29)
				EngineCore* this_window = reinterpret_cast<EngineCore*>(glfwGetWindowUserPointer(window));
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

	GLFWscrollfun EngineCore::SetScrollCallBackToGLFW()
	{
		GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			ZoneScopedN("InputSystemScrollUpdate")
				ZoneText("Updating Scrolling", 19)
				EngineCore* this_window = reinterpret_cast<EngineCore*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnScrollYAxis(yoffset);
			}
		};
		return glfwSetScrollCallback(m_window, scroll_callback);
	}

	GLFWwindowsizefun EngineCore::SetWindowSizeToGLFW()
	{
		GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
		{
			EngineCore* this_window = reinterpret_cast<EngineCore*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnResizeWindow(width, height);
			}
		};
		return glfwSetWindowSizeCallback(m_window, window_size_callback);
	}
}