#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>

#include "EditorEngine.h"
#include "GUIManager.h"
#include "PxQueryReport.h"
#include "PxRigidActor.h"
#include "PhysicsScene.h"

#include "WindowFileBrowser.h"
#include "WindowConsole.h"
#include "WindowInspector.h"
#include "WindowMaterial.h"
#include "WindowShader.h"
#include "WindowViewport.h"
#include "WindowScene.h"
#include "WindowHierarchy.h"
#include "WindowProfiler.h"
#include "MenuGroup.h"

#include "RootObject.hpp"
#include "ResourcesManager.h"
#include "Mesh.h"

namespace Editor
{
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


	EditorEngine::EditorEngine() : EditorEngine(1280, 800)
	{
	}

	EditorEngine::EditorEngine(int width, int height) : EngineCore(width, height)
	{
		ZoneScoped
		m_editorInput = new Core::SystemManagement::InputSystem(this);
	}

	EditorEngine::~EditorEngine()
	{
		ZoneScoped
		delete m_man;
		delete m_editorInput;
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
	void callback_error(int error, const char* message)
	{
		std::cerr << message << std::endl;
	}

	int EditorEngine::OnInit(const int width, const int height)
	{
		ZoneScoped
		glfwSetErrorCallback(callback_error);
		int temp;
		if (temp = EngineCore::OnInit(width, height))
			return temp;

		if (!glfwInit())
			return 201;

		if (!gladLoadGL())
			return 202;

		m_physicsScene->CreateQueryScene(m_editorScene);
		glDebugMessageCallback(MessageCallback, 0);

		glfwMakeContextCurrent(m_window);
		{
			ZoneScopedN("SetIcon")
			GLFWimage icons;
			icons.pixels = stbi_load("Resources\\Images\\bread.png", &icons.width, &icons.height, 0, STBI_rgb_alpha);
			glfwSetWindowIcon(m_window, 1, &icons);
			stbi_image_free(icons.pixels);
		}
		SetCallbackToGLFW();

		m_man = new Editor::GUIManager(this, Core::Datastructure::glsl_version, Editor::GUIStyle::BAKER_DARK);
		Editor::Canvas* canvas = new Editor::Canvas(m_man);
		m_man->SetCanvas(canvas);

		canvas->Add<Editor::Window::WindowHierarchy>();
		canvas->Add<Editor::Window::WindowInspector>();
		canvas->Add<Editor::Window::WindowMaterial>();
		canvas->Add<Editor::Window::WindowShader>();
		canvas->Add<Editor::Window::WindowViewport>();
		canvas->Add<Editor::Window::WindowScene>();
		canvas->Add<Editor::Window::WindowConsole>();
		canvas->Add<Editor::Window::WindowFileBrowser>();
		canvas->Add<Editor::Window::WindowProfiler>(false);

		return 0;
	}

	void EditorEngine::SetSizeWindow(const int width, const int height)
	{
		ZoneScoped
		m_width = width;
		m_height = height;
	}

	void EditorEngine::MainLoop()
	{
		while (!glfwWindowShouldClose(m_window))
		{
			OnLoop();
		}
	}

	void EditorEngine::EndFrame()
	{
		ZoneScoped
		if (objectSelected != nullptr && objectSelected->IsDestroyed())
			objectSelected = nullptr;
		UpdateMeshNeedToInit();
		EngineCore::EndFrame();
		m_editorInput->ClearRegisteredInputs();
	}

	void EditorEngine::OnLoop()
	{
		ZoneScoped
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		if (m_inputSystem->IsCursorHidden() || m_editorInput->IsCursorHidden())
			ImGui::SetMouseCursor(-1);
		if (m_editorInput->IsMouseButtonPressed(EMouseButton::LEFT))
			isTestingRay = true;
		switch (m_state)
		{
		case (Core::Datastructure::EngineState::STARTING):
			UpdateSavedScene();
		case (Core::Datastructure::EngineState::RUNNING):
			if (m_paused && !m_step)
			{
				Render();
				EndFrame();
				break;
			}
		case (Core::Datastructure::EngineState::CLOSING):
			m_step = false;
			EngineCore::OnLoop();
			break;
		case (Core::Datastructure::EngineState::CLOSED):
		{
			ZoneScopedN("EngineClosed")
			objectSelected = nullptr;
			ReloadScene();
			m_paused = false;
			m_state = Core::Datastructure::EngineState::INITIALIZED;
			Render();
			EndFrame();
			break;
		}
		case (Core::Datastructure::EngineState::INITIALIZED):
		{
			ZoneScopedN("EngineInitialized")
				m_manager->UpdateResourcesManager();
			Render();
			EndFrame();
			break;
		}
		default:
		{
			ZoneScopedN("Unknown engine state")
			break;
		}
		}

		{
			ZoneScopedN("Display and swap")
			int display_w, display_h;
			glfwGetFramebufferSize(m_window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);

			TRACY_GL_IMAGE_SEND(m_width, m_height)
				TracyGpuCollect
			glfwSwapBuffers(m_window);

			FrameMark
		}
	}

	void EditorEngine::Render()
	{
		ZoneScoped
		EngineCore::Render();
		m_man->Render();
	}

	bool EditorEngine::IsSelectingEngineView()
	{
		return m_man->IsWindowFocused(2);
	}

	void	EditorEngine::SelectObjectInScene(const Core::Maths::Vec3& origin, const Core::Maths::Vec3& direction)
	{
		physx::PxRaycastBuffer hit;
		Core::Datastructure::Object* result{ nullptr };
		physx::PxHitFlags flag = physx::PxHitFlag::ePOSITION | physx::PxHitFlag::eNORMAL | physx::PxHitFlag::eUV;
		if(m_editorScene->raycast(physx::PxVec3{ origin.x, origin.y, origin.z },
								physx::PxVec3{ direction.x, direction.y, direction.z }, FLT_MAX, hit, flag))
		{
			if (hit.hasBlock)
			{
				Core::Datastructure::IComponent* physicsMesh{ static_cast<Core::Datastructure::IComponent*>(hit.block.actor->userData) };
				if (physicsMesh)
					result = physicsMesh->GetParent();
			}
		}
		else
		{
			result = SearchObjectInScene(origin, direction);
		}

		if (result != nullptr)
			objectSelected = result;
	}

	void EditorEngine::InitMesh(Core::Renderer::Mesh* mesh)
	{
		if (!mesh->CreateAABBMesh(m_editorScene))
			m_meshesNeedInit.push_back(mesh);
	}

	void EditorEngine::PutBoxCollider(Core::Physics::Collider* coll)
	{
		m_boxCollider.push_back(coll);
	}

	void EditorEngine::DeleteBoxCollider(Core::Physics::Collider* coll)
	{
		m_boxCollider.remove(coll);
	}

	void EditorEngine::PutSphereCollider(Core::Physics::Collider* coll)
	{
		m_sphereCollider.push_back(coll);
	}

	void EditorEngine::DeleteSphereCollider(Core::Physics::Collider* coll)
	{
		m_sphereCollider.remove(coll);
	}

	void EditorEngine::PutCapsuleCollider(Core::Physics::Collider* coll)
	{
		m_capsuleCollider.push_back(coll);
	}

	void EditorEngine::DeleteCapsuleCollider(Core::Physics::Collider* coll)
	{
		m_capsuleCollider.remove(coll);
	}

	void EditorEngine::SaveObject3DInfo(const char* fileName, const Resources::Loader::Object3DInfo& Object3D)
	{
		m_info3DObject.emplace(fileName, std::make_shared<Resources::Loader::Object3DInfo>(Object3D));
	}

	std::shared_ptr<Resources::Loader::Object3DInfo> EditorEngine::GetObject3DInfo(const std::string& localPath)
	{
		if (m_info3DObject.count(localPath))
			return m_info3DObject[localPath];
		return nullptr;
	}

	Core::Maths::Vec2 EditorEngine::GetMousePos() noexcept
	{
		ZoneScoped
		double pos[2];
		glfwGetCursorPos(GetWindow(), &pos[0], &pos[1]);
		return Core::Maths::Vec2(static_cast<float>(pos[0]), static_cast<float>(pos[1]));
	}

	Core::Maths::Vec2 EditorEngine::GetMousePosInWindow() noexcept
	{
		ZoneScoped
			return relativeMousePos;
	}

	void EditorEngine::Play()
	{
		m_state = Core::Datastructure::EngineState::STARTING;
	}

	void EditorEngine::EndPlay()
	{
		m_state = Core::Datastructure::EngineState::CLOSING;
	}

	bool EditorEngine::IsPlaying()
	{
		return m_state >= Core::Datastructure::EngineState::STARTING && m_state <= Core::Datastructure::EngineState::CLOSING;
	}

	void EditorEngine::Pause()
	{
		m_paused = true;
	}

	void EditorEngine::Unpause()
	{
		m_paused = false;
	}

	void EditorEngine::TogglePause()
	{
		m_paused = !m_paused;
	}

	bool EditorEngine::IsPaused()
	{
		return m_paused;
	}

	void EditorEngine::Step()
	{
		m_step = true;
	}

	json	ClassToJson(rttr::type t, rttr::instance i);

	json	ValToJson(rttr::type t, rttr::variant val)
	{
		json out;
		out["Type"] = t.get_name().to_string();

		if (t.is_enumeration())
		{
			out["Value"] = val.get_value<int>();
		}
		else if (t == rttr::type::get<bool>())
		{
			out["Value"] = val.get_value<bool>();
		}
		else if (t == rttr::type::get<int>())
		{
			out["Value"] = val.get_value<int>();
		}
		else if (t == rttr::type::get<float>())
		{
			out["Value"] = val.get_value<float>();
		}
		else if (t == rttr::type::get<Core::Maths::Vec2>())
		{
			out["Value"] = val.get_value<Core::Maths::Vec2>().xy;
		}
		else if (t == rttr::type::get<Core::Maths::Vec3>())
		{
			out["Value"] = val.get_value<Core::Maths::Vec3>().xyz;
		}
		else if (t == rttr::type::get<Core::Maths::Vec4>())
		{
			out["Value"] = val.get_value<Core::Maths::Vec4>().xyzw;
		}
		else if (t == rttr::type::get<Core::Maths::Color>())
		{
			out["Value"] = val.get_value<Core::Maths::Color>().rgba;
		}
		else if (t == rttr::type::get<std::string>())
		{
			out["Value"] = val.get_value<std::string>();
		}
		else if (t.is_array() || val.is_sequential_container() || val.is_associative_container())
		{
			if (val.is_sequential_container())
			{
				out["Type"] = "ArraySequential";
				out["Value"] = json::array();
				auto view{ val.create_sequential_view() };
				out["Wrapped type"] = view.get_value_type().get_name();
				for (auto value : view)
					out["Value"].push_back(ValToJson(view.get_value_type(), value.extract_wrapped_value()));
			}
			else if (val.is_associative_container())
			{
				out["Type"] = "ArrayAssociative";
				BAKERS_LOG_WARNING("Associative array save is not yet implemented");
			}
			else
			{
				BAKERS_LOG_ERROR("Array is not working with RTTR");
			}
		}
		else if (t.is_class())
		{
			out["Value"] = ClassToJson(t, val);
		}
		else
		{
			out["Value"] = "Unknown: " + t.get_name().to_string() + ", please implement this save";
		}

		return out;
	}

	json	PropertyToJson(rttr::property prop, rttr::instance i)
	{
		rttr::type propType = prop.get_type();
		rttr::variant val = prop.get_value(i);

		json out{ ValToJson(propType, val) };

		out["Name"] = prop.get_name().to_string();

		return out;
	}

	json	ClassToJson(rttr::type t, rttr::instance i)
	{
		json out;
		out["Type"] = t.get_name().to_string();
		{
			json values;
			for (rttr::property p : t.get_properties(filter_item::instance_item | filter_item::non_public_access | filter_item::public_access))
			{
				if (p.is_readonly() || p.is_static())
					continue;
				values[p.get_name().to_string()] = PropertyToJson(p, i);
			}
			out["Values"] = values;
		}

		return out;
	}

	json	ObjectToJson(Core::Datastructure::Object* object)
	{
		json out;
		out["Name"] = object->GetName();
		out["Childs"] = json::array();
		out["Components"] = json::array();
		out["Flags"] = (unsigned char)object->GetFlags();
		{
			json v3;
			Core::Maths::Vec3 temp{ object->GetPos() };
			v3["x"] = temp.x;
			v3["y"] = temp.y;
			v3["z"] = temp.z;
			out["Pos"] = v3;
			temp = object->GetScale();
			v3["x"] = temp.x;
			v3["y"] = temp.y;
			v3["z"] = temp.z;
			out["Scale"] = v3;
		}
		{
			Core::Maths::Quat temp{ object->GetRot() };
			json quat;
			quat["x"] = temp.x;
			quat["y"] = temp.y;
			quat["z"] = temp.z;
			quat["w"] = temp.w;
			out["Rot"] = quat;
		}
		int i{ 0 };
		for (auto it : object->GetChildren())
			out["Childs"][i++] = ObjectToJson(it);
		i = 0;
		for (auto it : object->GetComponents())
			out["Components"][i++] = ClassToJson(it->get_type(), it);
		return out;
	}

	void EditorEngine::UpdateSavedScene()
	{
		ZoneScoped
		m_savedScene.clear();
		int i{ 0 };
		m_savedScene["Childs"] = json::array();
		for (auto it : m_root->GetChildren())
		{
			m_savedScene["Childs"][i++] = ObjectToJson(it);
		}
	}

	void EditorEngine::SaveScene()
	{
		ZoneScoped
		if (m_state >= Core::Datastructure::EngineState::STARTING && m_state <= Core::Datastructure::EngineState::CLOSING)
			return;
		UpdateSavedScene();
		{
			ZoneScopedN("Writing scene to file")
				ZoneText("This might leak as std::ostream apparently leaks", 49)
			std::ofstream o(m_currScene);
			o << std::setw(4) << m_savedScene << std::endl;
			o.close();
		}
		BAKERS_LOG_MESSAGE("Scene saved");
		m_navMesh->SaveNavMesh(m_currScene);
		GetResourcesManager()->SaveMaterial();
	}

	void EditorEngine::ReloadScene()
	{
		ZoneScoped
		m_root->Clear();
		m_root->RemoveDestroyed();
		LoadSceneFromJson(m_savedScene);
	}

	void EditorEngine::CompileNavMesh()
	{
		ZoneScoped
		m_navMesh->ClearInputMeshes();
		for (auto it : m_root->GetComponentsOfBaseType<Core::Renderer::Mesh>())
		{
			it->AddToNavMesh();
		}
		m_navMesh->Build();
	}

	void EditorEngine::SetGameInputState(bool state)
	{
		m_inputSystem->SetActive(state);
	}

	void EditorEngine::SetCallbackToGLFW()
	{
		ZoneScoped
		OnResizeWindow += BIND_EVENT_2(EditorEngine::SetSizeWindow);

		SetKeyCallBackToGLFW();
		SetMouseButtonCallBackToGLFW();
		SetScrollCallBackToGLFW();
		SetWindowSizeToGLFW();
	}

	GLFWkeyfun EditorEngine::SetKeyCallBackToGLFW()
	{
		ZoneScoped
		GLFWkeyfun key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			ZoneScopedN("InputSystemKeyUpdate")
				ZoneText("Updating key presses", 21)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
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

	GLFWmousebuttonfun EditorEngine::SetMouseButtonCallBackToGLFW()
	{
		ZoneScoped
		GLFWmousebuttonfun mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			ZoneScopedN("InputSystemMouseButtonUpdate")
				ZoneText("Updating MouseButton presses", 29)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
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

	GLFWscrollfun EditorEngine::SetScrollCallBackToGLFW()
	{
		ZoneScoped
		GLFWscrollfun scroll_callback = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			ZoneScopedN("InputSystemScrollUpdate")
				ZoneText("Updating Scrolling", 19)
				EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnScrollYAxis(yoffset);
			}
		};
		return glfwSetScrollCallback(m_window, scroll_callback);
	}

	GLFWwindowsizefun EditorEngine::SetWindowSizeToGLFW()
	{
		ZoneScoped
		GLFWwindowsizefun window_size_callback = [](GLFWwindow* window, int width, int height)
		{
			EditorEngine* this_window = reinterpret_cast<EditorEngine*>(glfwGetWindowUserPointer(window));
			if (this_window)
			{
				this_window->OnResizeWindow(width, height);
			}
		};
		return glfwSetWindowSizeCallback(m_window, window_size_callback);
	}

	void EditorEngine::UpdateMeshNeedToInit()
	{
		for (auto it = m_meshesNeedInit.begin(); it != m_meshesNeedInit.end();)
		{
			if (!(*it) || (*it)->IsDestroyed() || (*it)->CreateAABBMesh(m_editorScene))
				it = m_meshesNeedInit.erase(it);
			else
				++it;
		}
	}
}
