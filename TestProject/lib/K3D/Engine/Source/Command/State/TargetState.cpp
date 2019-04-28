#include "TargetState.h"

K3D::TargetState::TargetState()
{
}

K3D::TargetState::~TargetState()
{
}

void K3D::TargetState::AddRenderTargetHandle(D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	this->_rtvCPUHandles.push_back(handle);
}

void K3D::TargetState::SetDepthStencilHandle(D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	this->_dsvCPUHandle = handle;

}

void K3D::TargetState::AddViewport(D3D12_VIEWPORT && viewport)
{
	this->_viewports.push_back(std::move(viewport));

}

void K3D::TargetState::AddScissorRects(D3D12_RECT && scissor)
{
	this->_scissorRects.push_back(std::move(scissor));
}
