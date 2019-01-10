#pragma once
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/Resource/DepthStencilBuffer.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <unordered_map>
#include <memory>
namespace K3D {
	class CommandList;

	class GeometryBuffer
	{
		friend class RenderingManager;

	public:

		enum GEOMETRY_BUFFER_TYPE {
			GEOMETRY_BUFFER_TYPE_ALBEDO,
			GEOMETRY_BUFFER_TYPE_DIFFUSE,
			GEOMETRY_BUFFER_TYPE_SPECULER,
			GEOMETRY_BUFFER_TYPE_VIEW_NORMAL,
			GEOMETRY_BUFFER_TYPE_EMISSIVE,
			GEOMETRY_BUFFER_TYPE_DEPTH,
			GEOMETRY_BUFFER_TYPE_VELOCITY,
		};

	private:
		DescriptorHeap _geometryBufferHeap;

		DescriptorHeap _rendaerTargetHeap;

		std::unordered_map<GEOMETRY_BUFFER_TYPE, ShaderResource> _geometryBufferResource;

	public:

		GeometryBuffer();
	
		~GeometryBuffer();
	
		void Initialize(unsigned int windowWidth,unsigned int windowHeight);

		void Discard();

		D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle();

		D3D12_CPU_DESCRIPTOR_HANDLE GetTextureCPUHandle(GEOMETRY_BUFFER_TYPE type);

		D3D12_GPU_DESCRIPTOR_HANDLE GetTextureGPUHandle(GEOMETRY_BUFFER_TYPE type);

		D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetCPUHandle(GEOMETRY_BUFFER_TYPE type);

		D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTargetGPUHandle(GEOMETRY_BUFFER_TYPE type);

		void ResourceBarrier(std::weak_ptr<CommandList> cmd, GEOMETRY_BUFFER_TYPE type , D3D12_RESOURCE_STATES state);

	private:

		void InitializeHeap();
		
		void InitializeResource(unsigned int windowWidth, unsigned int windowHeight);
	
	};

}