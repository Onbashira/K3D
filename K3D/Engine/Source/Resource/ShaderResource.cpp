#include "ShaderResource.h"
#include "Engine/Source/CoreSystem/Framework.h"



K3D::ShaderResource::ShaderResource()
{
}


K3D::ShaderResource::~ShaderResource()
{
}

void K3D::ShaderResource::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC & srv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	Framework::GetDevice().GetDevice()->CreateShaderResourceView(this->GetResource().Get(), &srv, cpuDescriptorHandle);
}

void K3D::ShaderResource::CreateView(D3D12_RENDER_TARGET_VIEW_DESC & rtv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	Framework::GetDevice().GetDevice()->CreateRenderTargetView(this->GetResource().Get(), &rtv, cpuDescriptorHandle);
}

void K3D::ShaderResource::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC & uav, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, ID3D12Resource* counterResource)
{
	Framework::GetDevice().GetDevice()->CreateUnorderedAccessView(this->GetResource().Get(), counterResource, &uav, cpuDescriptorHandle);
}

UINT64 K3D::ShaderResource::GetWidth()
{
	return GetResource()->GetDesc().Width;
}

UINT64 K3D::ShaderResource::GetHeight()
{
	return GetResource()->GetDesc().Height;
}

void K3D::ShaderResource::Discard()
{

}



