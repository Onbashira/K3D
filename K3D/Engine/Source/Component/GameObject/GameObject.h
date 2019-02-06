#pragma once
#include "Transform.h"
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>


namespace K3D {

	class CommandList;

	class InputComponent;

	class GraphicsComponent;

	class PhysicsComponent;

	class GameObject
	{
	public:

	protected:

		bool _isEnable;

		Transform _transform;

		ConstantBuffer _transformBuffer;

		std::unique_ptr< InputComponent> _inputComponent;

		std::unique_ptr< GraphicsComponent> _graphicsComponent;

		std::unique_ptr< PhysicsComponent> _physicsComponent;


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
		//�o�b�t�@���\�[�X��Size����1�����o�b�t�@�Ƃ��ė̈���m�ۂ���G
		virtual void InitalizeTransformBuffer(size_t size);

	private:

	};

}