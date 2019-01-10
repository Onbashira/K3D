#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include "Resource.h"

namespace K3D {

	class ShaderResource : public K3D::Resource
	{
	private:

	public:	

		ShaderResource();
		
		virtual ~ShaderResource();

		void									CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC& srv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void									CreateView(D3D12_RENDER_TARGET_VIEW_DESC& rtv, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

		void									CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC& uav, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle, ID3D12Resource* counterResource = nullptr);

		UINT64									GetWidth();
		
		UINT64									GetHeight();
		
		void									Discard();
		
	};

}