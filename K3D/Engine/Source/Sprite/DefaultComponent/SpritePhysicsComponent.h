#pragma once
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"
namespace K3D {
	class GameObject;

	class SpritePhysicsComponent :public PhysicsComponent
	{
	public:
		SpritePhysicsComponent();
		virtual ~SpritePhysicsComponent();

		virtual void Update(GameObject* gameObject)override;

	};

}