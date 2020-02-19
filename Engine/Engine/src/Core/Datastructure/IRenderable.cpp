#include "IRenderable.hpp"
#include "RootObject.hpp"

void Core::Datastructure::IRenderable::OnStart()
{
	if (GetScene() == nullptr)
		return;
	GetScene()->AddRenderable(this);
}
