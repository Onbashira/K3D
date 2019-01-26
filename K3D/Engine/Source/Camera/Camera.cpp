#include "Camera.h"


K3D::Camera::Camera() :
	_mode(CAMERA_MODE::Perspective), _aspectRatio(0.0f)
{
}


K3D::Camera::~Camera()
{

}


void K3D::Camera::InitializeCamera(CAMERA_MODE type, const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	switch (type)
	{
	case CAMERA_MODE::Perspective:
		initializePerspective(width, height, nearClip, farClip, position, target, upWard);
		break;
	case CAMERA_MODE::Orthogonal:
		InitializeOrthogonal(width, height, nearClip, farClip, position, target, upWard);
		break;
	default:
		break;
	}
}

void K3D::Camera::InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{

	_mode = CAMERA_MODE::Perspective;

	_fov = DegToRad(fov);
	_near = nearClip;
	_far = farClip;
	_aspectRatio = width / height;
	_info.windowHeight = height;
	_info.windowWidth = width;

	auto mat = Matrix::ExtractRotationMatrix(Matrix::CreateLookAt(position, target, upWard));
	SetRotation(Quaternion::CreateFromRotationMatrix(mat));
	this->_projection = Matrix::CreatePerspectiveFOV(fov, _aspectRatio, nearClip, farClip);
}

HRESULT K3D::Camera::InitializeCameraDepthStencill(DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight)
{
	_depthStencillRersource.Discard();
	auto hr = _depthStencillRersource.Create(windowWidth, windowHeight, depthFormat, depthFormat);
	_depthStencillRersource.SetName("CameraDepthStencill");
	return hr;

}

void K3D::Camera::SetCameraParamater(std::weak_ptr<CommandList> list, unsigned int paramaterIndex)
{
	list.lock()->GetCommandList()->SetGraphicsRootConstantBufferView(paramaterIndex, _cameraMatrixBuffer.GetResource()->GetGPUVirtualAddress());
}

void K3D::Camera::Discard()
{

	_depthStencillRersource.Discard();
	_cameraMatrixBuffer.Discard();

}

HRESULT K3D::Camera::CreateBuffer()
{
	auto hr = _cameraMatrixBuffer.Create(1, true);
	if (FAILED(hr)) {
		return hr;
	}
	_cameraMatrixBuffer.SetName("CameraMatrixBuffer");
	return hr;
}

void K3D::Camera::ChangeCameraMode(CAMERA_MODE mode)
{

	switch (mode)
	{
	case K3D::CAMERA_MODE::Perspective:
		InitializeOrthogonal(_windowWidth, _windowHeight, this->_near, _far, this->GetPos(), this->GetPos() + GetLocalAxis().w, GetLocalAxis().v);
		break;
	case K3D::CAMERA_MODE::Orthogonal:
		initializePerspective(_windowWidth, _windowHeight, this->_near, _far, this->GetPos(), this->GetPos() + GetLocalAxis().w, GetLocalAxis().v);
		break;
	default:
		break;
	}

}

HRESULT K3D::Camera::InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{
	_mode = CAMERA_MODE::Orthogonal;

	_fov = 0;
	_near = nearClip;
	_far = farClip;
	_aspectRatio = width / height;
	_info.windowHeight = height;
	_info.windowWidth = width;

	SetScale(Vector3::one);
	SetPos(position);

	Matrix mat = Matrix::ExtractRotationMatrix(Matrix::CreateLookAt(position, target, upWard));
	SetRotation(Quaternion::CreateFromRotationMatrix(mat));

	this->_info.projection = this->_projection = Matrix::CreateOrthographic(width, height, nearClip, farClip);
	this->_info.view = Matrix::Invert(mat);
	this->_info.windowHeight = this->_windowHeight;
	this->_info.windowWidth = this->_windowWidth;

	if (FAILED(CreateBuffer())) {
		return E_FAIL;
	}
	Update();
	return S_OK;

}

HRESULT K3D::Camera::initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3 & position, const Vector3 & target, const Vector3 & upWard)
{

	_mode = CAMERA_MODE::Perspective;

	_fov = 0;
	_near = nearClip;
	_far = farClip;
	_aspectRatio = width / height;
	_info.windowHeight = height;
	_info.windowWidth = width;

	SetScale(Vector3::one);
	SetPos(position);

	Matrix mat = std::move(Matrix::CreateLookAt(position, target, upWard));
	SetRotation(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix(mat)));

	this->_info.projection = this->_projection = Matrix::CreatePerspectiveFOV(DegToRad(70), _aspectRatio, nearClip, farClip);;
	this->_info.view = Matrix::Invert(mat);
	this->_info.windowHeight = this->_windowHeight;
	this->_info.windowWidth = this->_windowWidth;

	if (FAILED(CreateBuffer())) {
		return E_FAIL;
	}

	Update();
	return S_OK;
}

K3D::CAMERA_MODE K3D::Camera::GetMode()
{
	return _mode;
}

const Matrix & K3D::Camera::GetProjection()
{
	return this->_projection;
}

const Matrix  K3D::Camera::GetViewProjection()
{
	return Matrix::Multiply(GetView(), _projection);
}

K3D::CameraInfo K3D::Camera::GetCameraInfo()
{
	return this->_info;
}

K3D::UploadBuffer<K3D::CameraInfo> & K3D::Camera::GetCameraBuffer()
{
	return this->_cameraMatrixBuffer;
}

K3D::DepthStencil & K3D::Camera::GetDepthStencil()
{
	return _depthStencillRersource;
}

float K3D::Camera::GetFov()
{
	return _fov;
}

float K3D::Camera::GetNearClip()
{
	return -(_projection._43 / _projection._33);
}

float K3D::Camera::GetFarClip()
{
	return -(_projection._43 / (_projection._33 - 1.0f));
}

const D3D12_VIEWPORT & K3D::Camera::GetViewport() const
{
	return _viewport;
}

const D3D12_RECT & K3D::Camera::GetScissorRect() const
{
	return _scissorRect;
}

void K3D::Camera::Update()
{
	CameraInfo cameraMat{};

	this->_info.projection = this->_projection;
	this->_info.view = this->GetView();
	this->_info.invView = Matrix::Invert(this->_info.view);
	this->_info.invViewProj = Matrix::Invert(this->_projection * this->GetView());

	this->_info.windowHeight = this->_windowHeight;
	this->_info.windowWidth = this->_windowWidth;

	_cameraMatrixBuffer.CopyData(0, cameraMat);
}

void K3D::Camera::DebugMove(K3D::InputManager & input)
{
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_UP)) {
		Move(Vector3::forward);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_DOWN)) {
		Move(Vector3::back);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_RIGHT)) {
		Move(Vector3::right);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_LEFT)) {
		Move(Vector3::left);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_SPACE)) {
		Move(Vector3::up);
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_SHIFT)) {
		Move(Vector3::down);
	}
}

void K3D::Camera::DebugRotate(InputManager & input)
{
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_W)) {
		RotationAxisAngles(GetLocalAxis().u, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_S)) {
		RotationAxisAngles(GetLocalAxis().u, DegToRad(-1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_D)) {
		RotationAxisAngles(GetLocalAxis().v, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_A)) {
		RotationAxisAngles(GetLocalAxis().v, DegToRad(-1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_Q)) {
		RotationAxisAngles(GetLocalAxis().w, DegToRad(1.0f));
	}
	if (input.IsDown(VIRTUAL_KEY_STATE::VKS_E)) {
		RotationAxisAngles(GetLocalAxis().w, DegToRad(-1.0f));
	}
}