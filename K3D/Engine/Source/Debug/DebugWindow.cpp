#include "DebugWindow.h"
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
	ImGuiIO& io = ImGui::GetIO();
	io.BackendRendererName = "imgui_in_d3d12";
	
	_device = dev;
	_fontCPUHandle = fontSrvCPUDescHandle;
	_fontGPUHandle = fontSrvGPUDescHandle;
			
}

void K3D::DebugWindow::ShutDown()
{
}

void K3D::DebugWindow::NewFrame()
{
}

void K3D::DebugWindow::RenderData(ImDrawData * drawData, std::weak_ptr<CommandList> list)
{
}

void K3D::DebugWindow::InvalidDataDevicePbject()
{
}

void K3D::DebugWindow::CreateDeviceObject()
{
}
