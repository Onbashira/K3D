#include "Transform.h"
#include <utility>

K3D::Transform::Transform() :
	_pos(), _rotation(Quaternion::CreateIdentity()),_scale(Vector3::one)
{

}


K3D::Transform::~Transform()
{
	_pos = Vector3::zero;
	_rotation = Quaternion::CreateIdentity();
}


void K3D::Transform::SetPos(const Vector3  pos)
{
	_pos = pos;
}

void K3D::Transform::SetRotation(const Quaternion rotation)
{
	_rotation = rotation;
	_eulerAngles = _rotation.EulerAngles();
}

void K3D::Transform::SetEulerAngles(const Vector3 eulerAugles)
{
	_eulerAngles = eulerAugles;
	_rotation = std::move(Quaternion::CreateFromEulerAngles(eulerAugles));
}

void K3D::Transform::SetScale(const Vector3 scale)
{
	_scale = scale;
}

Vector3 K3D::Transform::GetPos()
{
	return _pos;
}

Quaternion  K3D::Transform::GetRotation()
{
	return	_rotation;
}

Vector3 K3D::Transform::GetEulerAngles()
{
	return _rotation.EulerAngles();
}

Vector3 K3D::Transform::GetScale()
{
	return _scale;
}

void K3D::Transform::Translate(const Vector3 & velocity)
{
	_pos += velocity;
}

void K3D::Transform::Move(const Vector3 & velocity)
{
	_pos += Vector3::Rotate(velocity, _rotation);
}

void K3D::Transform::Rotation(const Quaternion & rot)
{
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D::Transform::RotationLocalWorld(const Quaternion & rot)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();
}

void K3D::Transform::RotationLocalWorld(const Vector3 & eulerAngles)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();
	auto uRot = std::move(Quaternion::CreateFromAxisAngles(axis.u, DegToRad(eulerAngles.x)));
	auto vRot = std::move(Quaternion::CreateFromAxisAngles(axis.v, DegToRad(eulerAngles.y)));
	auto wRot = std::move(Quaternion::CreateFromAxisAngles(axis.w, DegToRad(eulerAngles.z)));

}

void K3D::Transform::RotationEulerAngles(const Vector3 & eulerAngles)
{
	_eulerAngles += eulerAngles; 
	_rotation = std::move(Quaternion::CreateFromEulerAngles(_eulerAngles));
}

void K3D::Transform::RotationAxisAngles(const Vector3 & axis, float rad)
{
	_rotation *= std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void K3D::Transform::RotateAround(const Vector3 & point, const Quaternion & rot)
{
	auto v = std::move(_pos - point);
	auto vv = std::move(Vector3::Rotate(v, rot));
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
	_pos = std::move(vv + (point));
}

void K3D::Transform::RotateAround(const Vector3 & point, const Vector3 & axis, float rad)
{
	auto q = std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	RotateAround(point, q);
}


void K3D::Transform::LookAt(const Vector3 & target, const Vector3 & up)
{
	//ターゲットをローカル空間に持っていく
	Vector3 localTargetPos = target;
	localTargetPos = Vector3::Transform(localTargetPos, GetView());
	_rotation = std::move(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix((Matrix::CreateLookAt(Vector3::zero, localTargetPos, up)))).Normalize());
	_eulerAngles = _rotation.EulerAngles();
}

OrthonormalBasis K3D::Transform::GetLocalAxis()
{
	OrthonormalBasis orthonomal;
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));
	orthonomal.u = Vector3(mat.axisX).Normalize();
	orthonomal.v = Vector3(mat.axisY).Normalize();
	orthonomal.w = Vector3(mat.axisZ).Normalize();
	return orthonomal;
}

Matrix K3D::Transform::GetView()
{
	//回転行列
	auto mat = Matrix::CreateFromQuaternion(_rotation);

	mat._41 = _pos.x;
	mat._42 = _pos.y;
	mat._43 = _pos.z;
	mat._44 = 1.0f;

	mat = Matrix::Invert(mat);
	//mat.EpsilonCheck();

	return mat;
}

Matrix K3D::Transform::GetSRTMatrix()
{
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));

	mat = std::move(Matrix::CreateScaleMatrix(_scale) * mat);
	mat.axisW.x = _pos.x;
	mat.axisW.y = _pos.y;
	mat.axisW.z = _pos.z;
	mat.axisW.w = 1.0f;
	return mat;
}

