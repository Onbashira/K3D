#pragma once
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"

namespace K3D {
	class DefaultPhysicsComponent : public PhysicsComponent
	{
	public:
		DefaultPhysicsComponent();
		virtual ~DefaultPhysicsComponent();

		virtual void Update(GameObject* gameObject)override {};

	};

}