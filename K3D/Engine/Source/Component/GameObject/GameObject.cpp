#include "GameObject.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
#include "Engine/Source/Component/InputComponent/InputComponent.h"
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"

K3D::GameObject::GameObject(K3D::GraphicsComponent* graphicsComponent, 
	K3D::InputComponent* inputComponent,
	K3D::PhysicsComponent* physicsComponent,
	std::shared_ptr<GameHeap>& gameHeap):
	_transform({}), 
	_isEnable(true),
	_gameHeap(gameHeap),
	_graphicsComponent(graphicsComponent),
	_inputComponent(inputComponent),
	_physicsComponent(physicsComponent)
{

}

K3D::GameObject::~GameObject()
{
	_transformBuffer.Discard();
	
}


void K3D::GameObject::Enable()
{
	_isEnable = true;
}

void K3D::GameObject::Disable()
{
	_isEnable = false;
}

bool K3D::GameObject::IsEnable()
{
	return _isEnable;
}

K3D::Transform & K3D::GameObject::GetTransform()
{
	return _transform;
}

void K3D::GameObject::SetGameHeap(std::shared_ptr<GameHeap>& gameHeap)
{
	_gameHeap = gameHeap;
}

std::unique_ptr<K3D::GraphicsComponent>& K3D::GameObject::GetGraphicsComponent()
{
	return _graphicsComponent;
}

std::unique_ptr<K3D::InputComponent>& K3D::GameObject::GetInputComponent()
{
	return _inputComponent;
}

std::unique_ptr<K3D::PhysicsComponent>& K3D::GameObject::GetPhysicsComponent()
{
	return _physicsComponent;
}

void K3D::GameObject::UpdateTransformBuffer()
{
	auto mat = Matrix::CreateFromQuaternion(this->_transform.GetRotation());
	mat.axisW.x = _transform.GetPos().x;
	mat.axisW.y = _transform.GetPos().y;
	mat.axisW.z = _transform.GetPos().z;
	mat.axisW.w = 1.0f;

	mat = _transform.GetSRTMatrix();

	_transformBuffer.Update(&mat, _transformBuffer.GetInitializeSize(), 0);
}

void K3D::GameObject::InitalizeTransformBuffer(size_t size)
{
	_transformBuffer.Initialize(size);
}