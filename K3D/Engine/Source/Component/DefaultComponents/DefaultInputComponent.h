#pragma once
#include "Engine/Source/Component/InputComponent/InputComponent.h"


namespace K3D {
	class DefaultInputComponent : public InputComponent
	{
	public:
		DefaultInputComponent();
		virtual ~DefaultInputComponent();

		virtual void Update(GameObject* gameObject, InputManager* inputManager)override {};

	};

}