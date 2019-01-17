#pragma once
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/GameObject/GameObject.h"
#include "Engine/Source/Resource/UploadBuffer.h"
#include "Engine/Source/Resource/DepthStencilBuffer.h"

namespace K3D {

	class CommandList;

	enum class CameraMode {
		Perspective,
		Orthogonal
	};

	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invViewProj;
		float  windowWidth;
		float  windowHeight;
	};

	class Camera :
		public GameObject
	{

	public:

	private:
		CameraMode		_mode;

		float			_fov;
		float			_aspectRatio;
		float			_near;
		float			_far;

		float			_windowWidth;
		float			_windowHeight;

		D3D12_VIEWPORT	_viewport;

		D3D12_RECT		_scissorRect;

		Matrix			_projection;

		UploadBuffer<CameraInfo>	_cameraMatrixBuffer;
		DepthStencil	_depthStencillRersource;

		CameraInfo		_info;

	public:

		Camera();

		~Camera();

		CameraMode		GetMode();

		const Matrix&	GetProjection();

		const Matrix	GetViewProjection();

		CameraInfo		GetCameraInfo();

		K3D::UploadBuffer<K3D::CameraInfo>&  GetCameraBuffer();

		DepthStencil&	GetDepthStencil();

		float GetFov();

		float GetAspectratio;

		float GetNearClip();

		float GetFarClip();

		const D3D12_VIEWPORT& GetViewport()const;

		const D3D12_RECT& GetScissorRect()const;

		void Update();

		void DebugMove(InputManager& input);

		void DebugRotate(InputManager& input);

		void ChangeCameraMode(CameraMode mode);

		void InitializeCamera(CameraMode type, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		void InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		HRESULT InitializeCameraDepthStencill(DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight);

		void SetCameraParamater(std::weak_ptr<CommandList> list, unsigned int paramaterIndex = 0);

		void Discard();

	private:

		HRESULT CreateBuffer();

		HRESULT InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		HRESULT initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

	};

}