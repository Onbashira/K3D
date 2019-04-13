#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Camera/Camera.h"
#include "Engine/Source/Rendering/RenderingPass/RenderingPass.h"
#include "Engine/Source/Rendering/RenderingPass/RenderingPassHolder.h"


K3D::Scene::Scene(std::shared_ptr<RenderingDevice>& renderingDevice) :
	_renderingDevice(renderingDevice),
	_renderContext(Framework::GetInstance().GetRenderingManagre().CreateRenderContext()),
	_gameHeap(new GameHeap(renderingDevice->GetD3D12Device(), K3D::GameHeap::GameHeapDesc())),
	_mainCamera(new Camera(_gameHeap))

{
	//RenderingContextCreate

	unsigned int w = 0,h = 0;
	K3D::Framework::GetInstance().GetWindowSize(w,h);
	//カメラのイニシャライズ
	_mainCamera->InitializeCameraFOV(90.0f, static_cast<float>(w),static_cast<float>( h),
		0.001f, 1000.0f, Vector3(0.0f, 10.0f, -10.0f), Vector3(), Vector3::up);
	//カメラのデプスステンシルバッファのイニシャライズ
	_mainCamera->InitializeCameraDepthStencill(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, w, h);
	//カメラCPUビューの作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = _mainCamera->GetCameraBuffer().GetResource()->GetGPUVirtualAddress();
	desc.SizeInBytes = Util::ConstantBufferAlign(sizeof(CameraInfo));//512Byte
	_gameHeap->CreateCBView(desc);
	
}

K3D::Scene::~Scene()
{
	Discard();
}

void K3D::Scene::SceneBegin()
{
	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::BEGIN);
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list.lock());

	//マスターレンダーターゲットのクリア
	{
		_renderContext->GetSwapChain()->ClearScreen(list.lock());
	}

	//メインカメラデプスのクリア
	{
		_mainCamera->GetDepthStencil().ClearDepthStencil(list.lock());
	}

	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());
}

void K3D::Scene::ExecutePath()
{
}

void K3D::Scene::SceneEnd()
{


	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::END);
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list.lock());
	_renderContext->GetSwapChain()->SetStatePresent(list.lock());
	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());

	_renderContext->ExecuteCmdList3DQueue();

	_renderContext->Present(1, 0);
	
	//レンダーターゲットフリッピング
	_renderContext->Flip();

	_renderContext->WaitForGPU(_renderContext->GetCommandQueue().lock(), false);

	_renderContext->ClearCmdLists();

	_renderContext->ResetCurrentCommandAllocator();
	
}

void K3D::Scene::Discard()
{
	//!デスクリプタヒープ
	_gameHeap->Discard();
	_renderContext->Discard();;
	_mainCamera->Discard();
}

