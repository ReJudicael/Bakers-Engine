#include "RootObject.hpp"
#include "EngineCore.h"
#include "CoreMinimal.h"

RTTR_PLUGIN_REGISTRATION
{
	ZoneScopedN("Registering RTTR")
	ZoneText("Leak happening in this zone is from RTTR and is actually reflexion data. It is a purposeful leak", 98);
	using namespace Core::Datastructure;
	registration::class_<RootObject>("RootObject")
		.method("StartFrame", &RootObject::StartFrame)
		.method("Update", &RootObject::Update)
		.method("Render", &RootObject::Render)
		.method("RemoveDestroyed", &RootObject::RemoveDestroyed)
		.method("AddUpdatable", &RootObject::AddUpdatable)
		.method("AddRenderable", &RootObject::AddRenderable)
		.method("AddStart", &RootObject::AddStart)
		.method("AddCamera", &RootObject::AddCamera)
		.property_readonly("Input", &RootObject::GetInput)
		.method("RemoveUpdatable", &RootObject::RemoveUpdatable)
		.method("RemoveRenderable", &RootObject::RemoveRenderable)
		.method("RemoveCamera", &RootObject::RemoveCamera)
		.method("DestroyComponent", &RootObject::DestroyComponent)
		.method("DestroyObject", &RootObject::DestroyObject)
		.method("Destroy", &RootObject::Destroy)
		.property("Transform", &RootObject::m_transform, detail::protected_access());
}

namespace Core::Datastructure
{
	RootObject::RootObject(SystemManagement::InputSystem* inputSystem, EngineCore* engine) noexcept : Object("Root", {}, nullptr, this), m_engine{ engine }
	{
		m_transform.RequireUpdate();
		m_transform.UpdatePos();
		m_inputSystem = inputSystem;
	}

	void RootObject::StartFrame() noexcept
	{
		ZoneScoped
			ZoneText("Start of components done here", 30)
		for (auto it{ m_componentsToStart.begin() }; it != m_componentsToStart.end();)
		{
			(*it)->Start();
			if ((*it)->IsStarted())
				it = m_componentsToStart.erase(it);
			else
				++it;
		}
	}

	void RootObject::Update(float deltaTime) noexcept
	{
		ZoneScoped
			ZoneText("Updating of components done here", 33)
		RemoveUpdatables();
		for (auto it{ m_updatables.begin() }; it != m_updatables.end(); ++it)
			(*it)->Update(deltaTime);
		RemoveUpdatables();
	}

	void RootObject::Render(std::list<Core::Renderer::Framebuffer*>& fboList) const noexcept
	{
		ZoneScoped
			ZoneText("Render of components done here", 31)
			TracyGpuZone("Rendering all components")
		
		GLint PreviousFramebuffer;
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &PreviousFramebuffer);
		glClearColor(0, 0, 0, 1);
		for (auto it{ m_cameras.begin() }; it != m_cameras.end(); ++it)
		{
			(*it)->Draw(m_renderables);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, PreviousFramebuffer);
	}
	void RootObject::AddUpdatable(IUpdatable* i) noexcept
	{
		m_updatables.emplace_back(i);
	}
	void RootObject::AddRenderable(IRenderable* i, size_t priority) noexcept
	{
		if (priority == 0)
		{
			auto it = m_renderables.begin();
			std::advance(it, m_renderPriorityBegin);
			m_renderables.emplace(it, i);
			m_renderPriorityBegin++;
		}
		else
			m_renderables.emplace_back(i);
	}
	void RootObject::AddStart(IComponent* i) noexcept
	{
		m_componentsToStart.emplace_back(i);
	}
	void RootObject::AddCamera(ICamera* i) noexcept
	{
		m_cameras.emplace_back(i);
	}

	SystemManagement::InputSystem* RootObject::GetInput()
	{
		return m_inputSystem;
	}

	void RootObject::RemoveUpdatable(IUpdatable* i) noexcept
	{
		m_removedUpdatables.push_back(i);
	}

	void RootObject::RemoveUpdatables() noexcept
	{
		for (auto it : m_removedUpdatables)
			m_updatables.remove(it);

		m_removedUpdatables.clear();
	}

	void RootObject::RemoveRenderable(IRenderable* i, size_t priority) noexcept
	{
		m_renderables.remove(i);

		if (priority == 0 && m_renderPriorityBegin > 0)
			m_renderPriorityBegin--;
	}

	void RootObject::RemoveCamera(ICamera* i) noexcept
	{
		m_cameras.remove(i);
	}

	void RootObject::DestroyComponent(IComponent* i) noexcept
	{
		m_destroyedComponents.push_back(i);
	}
	void RootObject::DestroyObject(Object* i) noexcept
	{
		m_destroyedObjects.push_back(i);
	}
	
	void RootObject::Destroy()
	{
		ZoneScoped
			ZoneText("Destroying scene and all its objects and components", 52)
		for (auto it{ m_components.begin() }; it != m_components.end(); ++it)
			(*it)->Destroy();
		for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
			(*it)->Destroy();
	
		RemoveDestroyed();
		SetDestroyed();
	}

	void RootObject::RemoveDestroyed() noexcept
	{
		ZoneScoped
			ZoneText("Removing destroyed objects and components", 42)
		{
			ZoneScoped
				ZoneText("Removing of destroyed components done here", 43)
				for (auto it{ m_destroyedComponents.begin() }; it != m_destroyedComponents.end(); ++it)
					delete (*it);
			m_destroyedComponents.clear();
		}
		{
			ZoneScoped
				ZoneText("Removing of destroyed objects done here", 43)
				for (auto it{ m_destroyedObjects.begin() }; it != m_destroyedObjects.end(); ++it)
					delete (*it);
			m_destroyedObjects.clear();
		}
	}

	void RootObject::Clear()
	{
		for (auto it{ m_childs.begin() }; it != m_childs.end(); ++it)
			(*it)->Destroy();
		m_componentsToStart.clear();
	}

	RootObject* RootObject::CreateRootNode(SystemManagement::InputSystem* inputSystem, EngineCore* engine) noexcept
	{
		ZoneScoped
		return new RootObject(inputSystem, engine);
	}
}
