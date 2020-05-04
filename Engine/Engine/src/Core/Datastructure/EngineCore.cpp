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
#include "PhysicsScene.h"
#include "DynamicMesh.h"

#include "json.hpp"
using nlohmann::json;
#include <fstream>

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

	EngineCore::EngineCore(const int width, const int height) : m_width{ width }, m_height{ height }, m_fbo{ nullptr }, m_window{ nullptr }, m_manager{ nullptr }, m_physicsScene{ nullptr }
	{
		m_inputSystem = new Core::SystemManagement::InputSystem(this);
		m_navMesh = new Core::Navigation::NavMeshBuilder();
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
		m_state = Core::Datastructure::EngineState::INITIALIZING;
		int init{ OnInit(width, height) };
		if (init)
			return init;
		init = !LoadScene("Default.json");
		m_state = Core::Datastructure::EngineState::INITIALIZED;
		return init;
	}

	int EngineCore::OnInit(int width, int height)
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

		m_fbo.clear();
		m_manager = new Resources::Loader::ResourcesManager();
		m_manager->SetRootNode(m_root);
		/*
		Core::Datastructure::Object* camNode{ m_root->CreateChild("Camera", {}) };
		camNode->SetPos({ 0.f, 2.f, 5.f });
		camNode->SetRot({ 0.f, 0.f, 0.f });

		Renderer::PlayerCamera* c = new Renderer::PlayerCamera(1200.f / 700.f, 60.f, 0.1f, 100.f);
		camNode->AddComponent(c);

		Core::Renderer::Light* l = new Core::Renderer::Light();
		l->SetLightType(Core::Renderer::Light::ELightType::POINT);
		l->SetAmbient({ 0.3f, 0.3f, 0.3f });
		l->SetDiffuse({ 0.5f, 0.5f, 0.5f });
		l->SetSpecular({ 0.2f, 0.2f, 0.2f });
		l->SetAttenuation({ 0.3f, 0.02f, 0.002f });
		l->SetRange(1000.f);
		l->SetAngle(0.785f);
		l->SetAngleSmoothness(0.01f);
		camNode->AddComponent(l);

		Core::Datastructure::ScriptedComponent* s = new Core::Datastructure::ScriptedComponent("Resources\\Scripts\\DefaultScript.lua");
		camNode->AddComponent(s);

		umbreon->AddComponent(new Core::Physics::DynamicMesh());

		staticMesh->SetPos({ 0.f,-5.f,0.f });
		staticMesh->SetScale({ 5.f,1.f,5.f });
		Core::Physics::StaticMesh* staticmesh1 = new Core::Physics::StaticMesh();
		((Core::Physics::BoxCollider*)staticmesh1->GetCollider())->SetBoxHalfExtent({ 500.f, 1.f, 500.f });
		staticMesh->AddComponent(staticmesh1);




		umbreon->SetPos({ 0.f,3.f, 0.f });
		*/
		Core::Datastructure::Object* umbreon{ m_root->CreateChild("Umbreon", {}) };
		m_manager->Load3DObject("Resources/Models/Umbreon/UmbreonHighPoly.obj");
		Resources::Object3DGraph::CreateScene("Resources/Models/Umbreon/UmbreonHighPoly.obj", *m_manager, umbreon);

		Core::Datastructure::Object* dining_room{ m_root->CreateChild("DiningRoom", {}) };
		Core::Datastructure::Object* staticMesh{ dining_room->CreateChild("Static Mesh", {}) };

		m_manager->Load3DObject("Resources/Models/DiningRoom/dining_room.fbx");
		Resources::Object3DGraph::CreateScene("Resources/Models/DiningRoom/dining_room.fbx", *m_manager, dining_room);

		dining_room->SetScale({ 0.01f, 0.01f, 0.01f });
		return 0;
	}

	void	EngineCore::OnLoop()
	{
		ZoneNamedN(updateLoop, "Main loop iteration", true)
		
		double deltaTime = GetDeltaTime();
		if (m_state == EngineState::RUNNING)
		{
			StartFrame();

			Update(deltaTime);

			Render();
		}
		else if (m_state == EngineState::STARTING)
		{
			m_state = EngineState::RUNNING;
		}
		else if (m_state == EngineState::CLOSING)
		{
			m_state = EngineState::CLOSED;
		}

		EndFrame();
	}

	void	EngineCore::Update(double deltaTime)
	{
		OnUpdate(deltaTime);
	}

	bool EngineCore::LoadScene(const std::string& scene)
	{
		m_currScene = scene;
		return OnLoadScene();
	}

	void	LoadProperty(rttr::property prop, rttr::instance inst, json j)
	{
		rttr::type	t{ prop.get_type() };
		if (!prop.is_valid() || !t.is_valid())
		{
			BAKERS_LOG_WARNING("Property is invalid, maybe the loaded object has changed?");
			return;
		}
		if (t.is_enumeration())
			prop.set_value(inst, static_cast<int>(j["Value"]));
		else if (t == rttr::type::get<int>())
			prop.set_value(inst, static_cast<int>(j["Value"]));
		else if (t == rttr::type::get<float>())
			prop.set_value(inst, static_cast<float>(j["Value"]));
		else if (t == rttr::type::get<bool>())
			prop.set_value(inst, static_cast<bool>(j["Value"]));
		else if (t == rttr::type::get<Core::Maths::Vec2>())
			prop.set_value(inst, Core::Maths::Vec2(static_cast<float>(j["Value"][0]), static_cast<float>(j["Value"][1])));
		else if (t == rttr::type::get<Core::Maths::Vec3>())
			prop.set_value(inst, Core::Maths::Vec3(static_cast<float>(j["Value"][0]), static_cast<float>(j["Value"][1]), static_cast<float>(j["Value"][2])));
		else if (t == rttr::type::get<Core::Maths::Vec4>())
			prop.set_value(inst, Core::Maths::Vec4(static_cast<float>(j["Value"][0]), static_cast<float>(j["Value"][1]), static_cast<float>(j["Value"][2]), static_cast<float>(j["Value"][3])));
		else if (t == rttr::type::get<Core::Maths::Color>())
			prop.set_value(inst, Core::Maths::Color(static_cast<float>(j["Value"][0]), static_cast<float>(j["Value"][1]), static_cast<float>(j["Value"][2]), static_cast<float>(j["Value"][3])));
		else if (t == rttr::type::get<std::string>())
			prop.set_value(inst, static_cast<std::string>(j["Value"]));
		else if (t.is_class())
		{
			const rttr::variant& propVar{ prop.get_value(inst) };
			for (auto it : j["Value"]["Values"])
			{
				LoadProperty(t.get_property(it["Name"]), propVar, it);
			}
			prop.set_value(inst, propVar);
		}
		else
		{
			BAKERS_LOG_WARNING(std::string("No operation known for type ") + t.get_name());
		}
	}

	void	AddComponent(json j, Object* parent)
	{
		rttr::type cType{ rttr::type::get_by_name(j["Type"]) };
		if (!cType.is_valid() || !cType.is_derived_from<IComponent>())
			return;
		rttr::variant c{ cType.invoke("GetCopy", cType.create(), {}) };
		
		for (auto it : j["Values"])
		{
			rttr::property prop {cType.get_property(it["Name"])};
			LoadProperty(prop, c, it);
		}
		

		parent->AddComponent(c.get_value<ComponentBase*>());
	}

	void	AddChild(json j, Object* parent)
	{
		if (ObjectFlag(j["Flags"]).test_flag(ObjectFlags::DYNAMICALLY_GENERATED))
			return;
		Object* o{ parent->CreateChild(j["Name"], { {j["Pos"]["x"], j["Pos"]["y"], j["Pos"]["z"]}, {j["Rot"]["w"], j["Rot"]["x"], j["Rot"]["y"], j["Rot"]["z"]}, {j["Scale"]["x"], j["Scale"]["y"], j["Scale"]["z"]} }) };

		for (auto& childs : j["Childs"])
		{
			AddChild(childs, o);
		}

		for (auto& components : j["Components"])
		{
			AddComponent(components, o);
		}
	}

	bool	EngineCore::OnLoadScene()
	{
		std::ifstream inputScene(m_currScene);
		if (!inputScene.is_open())
			return false;
		json data;
		inputScene >> data;

		LoadSceneFromJson(data);

		return true;
	}

	void EngineCore::LoadSceneFromJson(json scene)
	{
		for (auto& childs : scene["Childs"])
			AddChild(childs, m_root);
	}

	void	EngineCore::OnUpdate(double deltaTime)
	{
		m_physicsScene->BeginSimulate(static_cast<float>(deltaTime));
		m_physicsScene->EndSimulate();
		m_root->Update(static_cast<float>(deltaTime));
	}

	Object* EngineCore::SearchObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& dir)
	{
		Core::Physics::HitResultQuery query;

		if (m_physicsScene->Raycast(origin, dir, query))
			return query.objectHit;

		return nullptr;
	}

	Core::SystemManagement::InputSystem* EngineCore::GetInputSystem()
	{
		return m_inputSystem;
	}

	int Core::Datastructure::EngineCore::GetFBONum(Core::Renderer::FBOType t)
	{
		int num{ 0 };
		auto it{ m_fbo.begin() };
		while (it != m_fbo.end())
		{
			if ((*it)->type == t)
				++num;
			++it;
		}
		return num;
	}

	Core::Renderer::Framebuffer* EngineCore::GetFBO(int num, Core::Renderer::FBOType type)
	{
		if (num >= m_fbo.size())
			return nullptr;
		auto it{ m_fbo.begin() };
		int i = 0;
		while (true)
		{
			if (it == m_fbo.end())
				return nullptr;
			if ((*it)->type == type)
			{
				if (i == num)
					return *it;
				else
					++i;
			}
			++it;
		}
	}

	GLFWwindow* EngineCore::GetWindow()
	{
		return m_window;
	}

	void		EngineCore::StartFrame()
	{
		OnStartFrame();
	}

	void		EngineCore::OnStartFrame()
	{
		ZoneScoped
			m_root->StartFrame();
			m_manager->LinkAllTextureToOpenGl();
			m_manager->LinkAllModelToOpenGl();
			m_manager->ShaderUpdate();
			if (!m_navMesh->IsNavmeshUpdated())
				m_navMesh->Build();
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
		m_root->UpdateTransforms();
		
		m_inputSystem->ClearRegisteredInputs();
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO()
	{
		return CreateFBO(m_width, m_height);
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO(int width, int height, Core::Renderer::FBOType t)
	{
		return m_fbo.emplace_back(new Core::Renderer::Framebuffer(width, height, t));
	}

	void EngineCore::DeleteFBO(Core::Renderer::Framebuffer* fbo)
	{
		for (auto it{ m_fbo.begin() }; it != m_fbo.end();)
		{
			if (*it == fbo)
			{
				it = m_fbo.erase(it);
				delete fbo;
			}
			else
				++it;
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

	void EngineCore::AddMeshToNav(Vertex* verts, int nverts, GLuint* tris, int ntris, const Core::Datastructure::Transform& position)
	{
		std::vector<float>	vertices;
		vertices.reserve(nverts * 3);
		for (int i{ 0 }; i < nverts; ++i)
		{
			vertices.push_back(verts[i].Position.x);
			vertices.push_back(verts[i].Position.y);
			vertices.push_back(verts[i].Position.z);
		}
		std::vector<int>	indices;
		indices.reserve(ntris);
		for (int i{ 0 }; i < ntris; ++i)
		{
			indices.push_back(tris[i]);
		}
		m_navMesh->AddMesh(vertices.data(), nverts * 3, indices.data(), ntris, position);
	}
}