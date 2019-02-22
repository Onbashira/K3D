#pragma once
#include "Engine/Source/Resource/ConstantBuffer.h"
#include "Engine/Source/Component/Transform/Transform.h"
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
#include "Engine/Source/Component/InputComponent/InputComponent.h"
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"


#include <memory>


namespace K3D {

	class CommandList;

	class GameObject
	{
	public:

	protected:

		bool _isEnable;

		Transform _transform;

		ConstantBuffer _transformBuffer;

		std::unique_ptr<InputComponent> _inputComponent;

		std::unique_ptr<GraphicsComponent> _graphicsComponent;

		std::unique_ptr<PhysicsComponent> _physicsComponent;

	private:

	public:

		GameObject();

		virtual ~GameObject();

		virtual void Update() = 0;

		void Enable();

		void Disable();

		bool IsEnable();

		Transform& Transform();

		virtual std::unique_ptr<GraphicsComponent>& GraphicsComponent() = 0;

		virtual std::unique_ptr<InputComponent>& InputComponent() = 0;

		virtual std::unique_ptr<PhysicsComponent>& PhysicsComponent() = 0;

	protected:

		virtual void UpdateTransformBuffer();
		//バッファリソースにSize分の1次元バッファとして領域を確保する；
		virtual void InitalizeTransformBuffer(size_t size);

	private:

	};

}
