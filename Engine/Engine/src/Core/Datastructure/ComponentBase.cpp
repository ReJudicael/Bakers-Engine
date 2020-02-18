#include "ComponentBase.h"

void Core::Datastructure::ComponentBase::Start()
{
	if (m_isInit || !m_isActive)
		return;
	OnStart();
	m_isInit = true;
}

void Core::Datastructure::ComponentBase::Destroy()
{
	if (m_isDestroyed)
		return;
	OnDestroy();
	m_isDestroyed = true;
}
