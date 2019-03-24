#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Camera/Camera.h"


K3D::Scene::Scene(std::shared_ptr<RenderingDevice> renderingDevice, std::shared_ptr<RenderContext> renderContext) :
	_renderingDevice(renderingDevice), _renderContext(renderContext),
	_gameHeap(new GameHeap(renderingDevice->GetD3D12Device(), K3D::GameHeap::GameHeapDesc())),
	_mainCamera(new Camera(_gameHeap)) ,_name("TEST")

{
	unsigned int w = 0,h = 0;
	K3D::Framework::GetInstance().GetWindowSize(w,h);
	_mainCamera->InitializeCameraFOV(90.0f, w, h,
		0.001f, 1000.0f, Vector3(0.0f, 10.0f, -10.0f), Vector3(), Vector3::up);

	_mainCamera->InitializeCameraDepthStencill(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, w, h);
	//ƒJƒƒ‰CPUƒrƒ…[‚Ìì¬
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = _mainCamera->GetCameraBuffer().GetResource()->GetGPUVirtualAddress();
	desc.SizeInBytes = Util::ConstantBufferAlign(sizeof(CameraInfo));//512Byte
	_gameHeap->CreateCBView(desc);
	
}

K3D::Scene::~Scene()
{
}

