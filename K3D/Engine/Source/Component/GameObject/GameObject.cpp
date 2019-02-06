#include "GameObject.h"
#include "Engine/Source/CommandList/CommandList.h"

K3D::GameObject::GameObject() :
	_transform({}), _isEnable(true)
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

K3D::Transform & K3D::GameObject::Transform()
{
	return _transform;
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
	_transformBuffer.Create(size);
}