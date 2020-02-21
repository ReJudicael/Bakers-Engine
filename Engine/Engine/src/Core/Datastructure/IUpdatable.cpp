#include "IUpdatable.hpp"
#include "RootObject.hpp"

namespace Core::Datastructure
{
	void Datastructure::IUpdatable::OnStart()
	{
		if (GetScene() == nullptr)
			return;
		GetScene()->AddUpdatable(this);
	}
}