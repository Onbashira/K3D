#pragma once
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/Resource/DepthStencilBuffer.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <map>
#include <unordered_map>
#include <memory>

namespace K3D {
	class DecaleBuffer
	{
		friend class RenderingManager;

	public:
		DescriptorHeap _decalHeap;
		std::unordered_map<unsigned char, ShaderResource> _layeredDecalResoruce;
	private:

	public:

		DecaleBuffer();

		~DecaleBuffer();

		void Initialize(unsigned int windowWidth, unsigned int windowHeight);

		void Discard();

		D3D12_CPU_DESCRIPTOR_HANDLE GetTextureCPUHandle(unsigned char layer);

		D3D12_GPU_DESCRIPTOR_HANDLE GetTextureGPUHandle(unsigned char layer);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetCPUHandle(unsigned char layer);

		D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTargetGPUHandle(unsigned char layer);

		void ResourceBarrier(std::weak_ptr<CommandList> cmd, unsigned char layer, D3D12_RESOURCE_STATES state);

	private:

		void InitializeHeap();

		void InitializeResource(unsigned int windowWidth, unsigned int windowHeight);
	};

}