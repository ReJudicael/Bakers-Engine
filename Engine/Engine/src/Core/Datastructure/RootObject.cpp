#include "RootObject.hpp"
#include "Debug.h"

namespace Core::Datastructure
{
	RootObject::RootObject(SystemManagement::InputSystem* inputSystem) noexcept : Object({}, nullptr, this)
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
			if ((*it)->IsInit())
				it = m_componentsToStart.erase(it);
			else
				++it;
		}
	}

	void RootObject::Update(float deltaTime) const noexcept
	{
		ZoneScoped
			ZoneText("Updating of components done here", 33)
		for (auto it{ m_updatables.begin() }; it != m_updatables.end(); ++it)
			(*it)->Update(deltaTime);
	}

	void RootObject::Render() const noexcept
	{
		ZoneScoped
			ZoneText("Render of components done here", 31)
		for (auto it{ m_renderables.begin() }; it != m_renderables.end(); ++it)
			(*it)->Draw(m_cameras);
	}
	void RootObject::AddUpdatable(IUpdatable* i) noexcept
	{
		m_updatables.emplace_back(i);
	}
	void RootObject::AddRenderable(IRenderable* i) noexcept
	{
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
		m_updatables.remove(i);
	}
	void RootObject::RemoveRenderable(IRenderable* i) noexcept
	{
		m_renderables.remove(i);
	}

	void RootObject::RemoveCamera(ICamera* i) noexcept
	{
		m_cameras.remove(i);
	}

	void RootObject::SetCamerasRatio(float ratio) noexcept
	{
		for (auto value : m_cameras)
		{
			value->SetRatio(ratio);
		}
	}

	void RootObject::DestroyComponent(IComponent* i) noexcept
	{
		m_destroyedComponents.emplace_back(i);
	}
	void RootObject::DestroyObject(Object* i) noexcept
	{
		m_destroyedObjects.emplace_back(i);
	}
	void RootObject::RemoveDestroyed() noexcept
	{
		ZoneScoped
			ZoneText("Removing of destroyed components done here", 43)
		for (auto it{ m_destroyedComponents.begin() }; it != m_destroyedComponents.end(); ++it)
			delete* it;
		m_destroyedComponents.clear();
		for (auto it{ m_destroyedObjects.begin() }; it != m_destroyedObjects.end(); ++it)
			delete* it;
		m_destroyedObjects.clear();
	}

	RootObject* RootObject::CreateRootNode(SystemManagement::InputSystem* inputSystem) noexcept
	{
		return new RootObject(inputSystem);
	}
}
