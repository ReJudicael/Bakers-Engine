#include "EngineCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

#include "RootObject.hpp"
#include "ResourcesManager.h"
#include "PlayerCamera.h"
#include "ScriptedComponent.h"
#include "Framebuffer.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "PhysicsScene.h"
#include "RigidBody.h"
#include "AudioSystem.h"

#include "json.hpp"
using nlohmann::json;


RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
		ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
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
		ZoneScoped
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - m_time;

		m_time = currentTime;

		return deltaTime;
	}

	EngineCore::EngineCore() : EngineCore(1280, 800)
	{}

	EngineCore::EngineCore(const int width, const int height) : m_width{ width }, m_height{ height }, m_fbo{ nullptr }, m_window{ nullptr }, m_manager{ nullptr }, m_physicsScene{ nullptr }
	{
		ZoneScoped
		m_inputSystem = new Core::SystemManagement::InputSystem(this);
		m_root = Core::Datastructure::RootObject::CreateRootNode(m_inputSystem, this);
		m_audioSystem = new Audio::AudioSystem();
		m_navMesh = new Core::Navigation::NavMeshBuilder(this);
	}

	EngineCore::~EngineCore()
	{
		ZoneScoped
		m_root->RemoveDestroyed();
		delete m_root;
		delete m_inputSystem;
		for (auto fbo : m_fbo)
			delete fbo;
		if (m_manager)
			delete m_manager;
		Resources::Shader::lights.clear();
		Resources::Shader::lights.shrink_to_fit();
		if (m_physicsScene)
			delete m_physicsScene;
		delete m_navMesh;
		if (m_task)
			delete m_task;
		delete m_audioSystem;
		FREE_TRACY_GL_IMAGE
		Core::Debug::Logger::ClearLogs(); 
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
		ZoneScoped
		m_state = Core::Datastructure::EngineState::INITIALIZING;
		int init{ OnInit(width, height) };
		if (init)
			return init;
		LoadScene(m_currScene);
		m_state = Core::Datastructure::EngineState::INITIALIZED;
		return init;
	}

	Core::Datastructure::Object* EngineCore::AddMesh(const char* name,const char* model, const char* shader, const char* tex, const Transform& trs, Object* parent)
	{
		ZoneScoped
		Object* object;
		if (parent)
			object = parent->CreateChild(name, {});
		else
			object = m_root->CreateChild(name, {});

		Core::Renderer::Mesh* m = new Core::Renderer::Mesh();
		m->AddModel(GetResourcesManager()->GetModel(model));
		std::shared_ptr<Resources::Material> mat = std::make_shared<Resources::Material>();
		mat->shader = GetResourcesManager()->GetShader(shader);
		std::shared_ptr<Resources::Texture> texture;
		GetResourcesManager()->LoadTexture(tex, texture);
		mat->textures.push_back(texture);
		m->AddMaterial(mat);
		object->AddComponent(m);

		object->SetPos(trs.GetLocalPos());
		object->SetRot(trs.GetLocalRot());
		object->SetScale(trs.GetLocalScale());

		return object;
	}

	void EngineCore::CreateSkybox(bool cube)
	{
		if (cube)
		{
			Core::Datastructure::Object* skybox{ m_root->CreateChild("Skybox", {}) };
			AddMesh("Sky1", "Quad", "Skybox", "Resources/Textures/skybox1.jpg",
				{ {-50.f, 0.f, 0.f}, {0.f, static_cast<float>(M_PI_2), 0.f}, {100.f, 100.f, 0.f} }, skybox);
			AddMesh("Sky2", "Quad", "Skybox", "Resources/Textures/skybox2.jpg",
				{ {0.f, 49.f, 0.f}, {static_cast<float>(M_PI_2), 0.f, 0.f}, {100.f, 100.f, 0.f} }, skybox);
			AddMesh("Sky3", "Quad", "Skybox", "Resources/Textures/skybox3.jpg",
				{ {0.f, 0.f, -49.f}, {0.f, 0.f, 0.f}, {100.f, 100.f, 0.f} }, skybox);
			AddMesh("Sky4", "Quad", "Skybox", "Resources/Textures/skybox4.jpg",
				{ {0.f, -49.f, 0.f}, {-static_cast<float>(M_PI_2), 0.f, 0.f}, {100.f, 100.f, 0.f} }, skybox);
			AddMesh("Sky5", "Quad", "Skybox", "Resources/Textures/skybox5.jpg",
				{ {50.f, 0.f, 0.f}, {0.f, -static_cast<float>(M_PI_2), 0.f}, {100.f, 100.f, 0.f} }, skybox);
			AddMesh("Sky6", "Quad", "Skybox", "Resources/Textures/skybox6.jpg",
				{ {0.f, 0.f, 49.f}, {0.f, static_cast<float>(M_PI_2), 0.f}, {100.f, 100.f, 0.f} }, skybox);
		}
		else
		{
			AddMesh("Skysphere", "Sphere", "Skybox", "Resources/Textures/Skysphere.png",
				{ {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {50.f, 50.f, 50.f} });
		}
	}

	std::shared_ptr<Resources::Object3DGraph> EngineCore::LoadObject(const char* fileName, const bool graphInMulti)
	{
		Resources::Loader::Object3DInfo info = m_manager->Load3DObject(fileName, graphInMulti);

		if (m_manager->GetCountScene(fileName))
		{
			std::shared_ptr<Resources::Object3DGraph> object3D{ m_manager->GetScene(fileName) };
			SaveObject3DInfo(fileName, info);
			return object3D;
		}
		return nullptr;
	}

	Resources::Loader::ResourcesManager* EngineCore::GetResourcesManager()
	{
		return m_manager;
	}

	Core::Physics::PhysicsScene* EngineCore::GetPhysicsScene()
	{
		return m_physicsScene;
	}

	int EngineCore::OnInit(int width, int height)
	{
		ZoneScoped
		glfwSetErrorCallback(error);

		if (!glfwInit())
			return 1;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		m_window = glfwCreateWindow(width, height, "Bakers Engine", nullptr, nullptr);
		if (m_window == nullptr)
			return 2;

		glfwGetWindowSize(m_window, &m_width, &m_height);

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
			INIT_TRACY_GL_IMAGE(320, 180)
		m_physicsScene = new Core::Physics::PhysicsScene();

		if (!m_physicsScene->InitPhysX())
			return -1;

		m_fbo.clear();

		m_task = new Core::SystemManagement::TaskSystem();
		m_manager = new Resources::Loader::ResourcesManager(m_task);
		m_manager->SetRootNode(m_root);

		return 0;
	}

	void	EngineCore::OnLoop()
	{
		ZoneScoped
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
			Resources::Shader::lights.clear();
			m_state = EngineState::CLOSED;
		}

		EndFrame();
	}

	void	EngineCore::Update(double deltaTime)
	{
		ZoneScoped
		OnUpdate(deltaTime);
	}

	bool EngineCore::LoadScene(const std::string& scene)
	{
		ZoneScoped
		m_currScene = scene;
		m_root->Clear();
		return OnLoadScene();
	}

	void	LoadArray(json& j, rttr::variant_sequential_view out)
	{
		for (auto it : j["Value"])
		{
			if (j["Wrapped type"] == "int")
				out.insert(out.end(), static_cast<int>(it["Value"]));
			else if (j["Wrapped type"] == "float")
				out.insert(out.end(), static_cast<float>(it["Value"]));
			else if (j["Wrapped type"] == "bool")
				out.insert(out.end(), static_cast<bool>(it["Value"]));
			else if (j["Wrapped type"] == "Vec2")
				out.insert(out.end(), Core::Maths::Vec2(static_cast<float>(it["Value"][0]), static_cast<float>(it["Value"][1])));
			else if (j["Wrapped type"] == "Vec3")
				out.insert(out.end(), Core::Maths::Vec3(static_cast<float>(it["Value"][0]), static_cast<float>(it["Value"][1]), static_cast<float>(it["Value"][2])));
			else if (j["Wrapped type"] == "Vec4")
				out.insert(out.end(), Core::Maths::Vec4(static_cast<float>(it["Value"][0]), static_cast<float>(it["Value"][1]), static_cast<float>(it["Value"][2]), static_cast<float>(it["Value"][3])));
			else if (j["Wrapped type"] == "Color")
				out.insert(out.end(), Core::Maths::Color(static_cast<float>(it["Value"][0]), static_cast<float>(it["Value"][1]), static_cast<float>(it["Value"][2]), static_cast<float>(it["Value"][3])));
			else if (j["Wrapped type"] == "std::string")
				out.insert(out.end(), std::string(it["Value"]));
		}
	}

	void	LoadProperty(rttr::property prop, rttr::instance inst, json& j)
	{
		if (j["Type"] == "ArraySequential")
		{
			auto test = prop.get_value(inst);
			LoadArray(j, test.create_sequential_view());
			prop.set_value(inst, test);
			return;
		}
		rttr::type	t{ prop.get_type() };
		if (!prop.is_valid() || !t.is_valid())
		{
			BAKERS_LOG_WARNING("Property is invalid, maybe the loaded object has changed?");
			return;
		}
		if (t.is_enumeration())
			prop.set_value(inst, t.get_enumeration().name_to_value(t.get_enumeration().value_to_name(static_cast<int>(j["Value"]))));
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

	void	AddComponent(json& j, Object* parent)
	{
		ZoneScoped
			ZoneText("This zone might leak some data for next RTTR operations. This is intentional, and a purposeful leak.", 127)
		rttr::type cType{ rttr::type::get_by_name(j["Type"]) };
		if (!cType.is_valid() || !cType.is_derived_from<IComponent>())
			return;
		rttr::variant c{ cType.invoke("GetCopy", cType.create(), {}) };
		
		for (auto& it : j["Values"])
		{
			rttr::property prop {cType.get_property(it["Name"])};
			LoadProperty(prop, c, it);
		}
		
		parent->AddComponent(c.get_value<ComponentBase*>());
	}

	void	AddChild(json& j, Object* parent)
	{
		ZoneScoped
		if (ObjectFlag(j["Flags"]).test_flag(ObjectFlags::DYNAMICALLY_GENERATED))
			return;

		Object* o{ parent->CreateChild(j["Name"], { {j["Pos"]["x"], j["Pos"]["y"], j["Pos"]["z"]}, {j["Rot"]["w"], j["Rot"]["x"], j["Rot"]["y"], j["Rot"]["z"]}, {j["Scale"]["x"], j["Scale"]["y"], j["Scale"]["z"]} }) };
		{
			ZoneScopedN("Setting values flag via RTTR")
				ZoneText("This zone might leak some RTTR data once as it generates them. It is intended in RTTR, and is purposeful", 105)
			rttr::type::get<Core::Datastructure::Object>().get_property("flags").set_value(o, static_cast<Core::Datastructure::ObjectFlag>(static_cast<int>(j["Flags"])));
		}

		for (auto& components : j["Components"])
			AddComponent(components, o);

		for (auto& childs : j["Childs"])
			AddChild(childs, o);
	}

	bool	EngineCore::OnLoadScene()
	{
		ZoneScoped
		std::ifstream inputScene(m_currScene);
		if (!inputScene.is_open())
			return false;
		json data;
		inputScene >> data;

		LoadSceneFromJson(data);

		m_navMesh->LoadNavMesh(m_currScene);

		return true;
	}


	void EngineCore::LoadSceneFromJson(json& scene)
	{
		ZoneScoped
		for (auto& childs : scene["Childs"])
			AddChild(childs, m_root);
	}

	void	EngineCore::OnUpdate(double deltaTime)
	{
		ZoneScoped
		m_physicsScene->BeginSimulate(static_cast<float>(deltaTime));
		m_physicsScene->EndSimulate();
		m_navMesh->UpdateQuery();
		m_root->Update(static_cast<float>(deltaTime));
	}

	Object* EngineCore::SearchObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& dir)
	{
		ZoneScoped
		Core::Physics::HitResultQuery query;

		if (m_physicsScene->Raycast(origin, dir, query))
			return query.objectHit;

		return nullptr;
	}

	Core::SystemManagement::InputSystem* EngineCore::GetInputSystem()
	{
		return m_inputSystem;
	}

	Core::Audio::AudioSystem* EngineCore::GetAudioSystem()
	{
		return m_audioSystem;
	}

	int Core::Datastructure::EngineCore::GetFBONum(Core::Renderer::FBOType t)
	{
		ZoneScoped
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
		ZoneScoped
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
		ZoneScoped
		OnStartFrame();
	}

	void		EngineCore::OnStartFrame()
	{
		ZoneScoped
			m_root->StartFrame();
			m_manager->UpdateResourcesManager();
	}

	void		EngineCore::Render()
	{
		ZoneScoped
			m_root->Render(m_fbo);
	}
	
	void		EngineCore::EndFrame()
	{
		ZoneScoped
		m_root->RemoveDestroyed();
		m_root->UpdateTransforms();
		m_audioSystem->Tick();
		m_inputSystem->ClearRegisteredInputs();
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO()
	{
		ZoneScoped
		return CreateFBO(m_width, m_height);
	}

	Core::Renderer::Framebuffer* EngineCore::CreateFBO(int width, int height, Core::Renderer::FBOType t)
	{
		ZoneScoped
		return m_fbo.emplace_back(new Core::Renderer::Framebuffer(width, height, t));
	}

	void EngineCore::DeleteFBO(Core::Renderer::Framebuffer* fbo)
	{
		ZoneScoped
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
		ZoneScoped
		SetKeyCallBackToGLFW();
		SetMouseButtonCallBackToGLFW();
		SetScrollCallBackToGLFW();
		SetWindowSizeToGLFW();
	}

	GLFWkeyfun EngineCore::SetKeyCallBackToGLFW()
	{
		ZoneScoped
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
		ZoneScoped
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
		ZoneScoped
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
		ZoneScoped
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
		ZoneScoped
		if (ntris == 0 || nverts == 0)
			return;
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