#include "DebugWindow.h"
#include "Engine/ThirdParty/imgui/imgui_impl_dx12.h"
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Device/D3D12Device.h"



K3D::DebugWindow::DebugWindow()
{
}


K3D::DebugWindow::~DebugWindow()
{
}

void K3D::DebugWindow::Init(D3D12Device * dev, int frame, DXGI_FORMAT format, D3D12_CPU_DESCRIPTOR_HANDLE fontSrvCPUDescHandle, D3D12_GPU_DESCRIPTOR_HANDLE fontSrvGPUDescHandle)
{
	ImGui_ImplDX12_Init(dev->GetDevice().Get(), frame, format, fontSrvCPUDescHandle, fontSrvGPUDescHandle);
}

void K3D::DebugWindow::ShutDown()
{
	ImGui_ImplDX12_Shutdown();
}

void K3D::DebugWindow::NewFrame()
{
	ImGui_ImplDX12_NewFrame();
}

void K3D::DebugWindow::RenderData(ImDrawData * drawData, std::weak_ptr<CommandList> list)
{
	ImGui_ImplDX12_RenderDrawData(drawData, list.lock()->GetCommandList().Get());
}

void K3D::DebugWindow::InvalidDataDevicePbject()
{
	ImGui_ImplDX12_InvalidateDeviceObjects();
}

void K3D::DebugWindow::CreateDeviceObject()
{
	ImGui_ImplDX12_CreateDeviceObjects();
}
