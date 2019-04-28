#pragma once
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"
namespace K3D {
	class GameObject;

	class PrimitivePhysicsComponent :public PhysicsComponent
	{
	public:
		PrimitivePhysicsComponent();
		virtual ~PrimitivePhysicsComponent();

		virtual void Update(GameObject* gameObject)override;

	};

}