#include "EngineCore.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RootObject.hpp"

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

	int EngineCore::Init()
	{
		return Init(m_width, m_height);
	}
}