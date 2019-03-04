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


	private:
		DescriptorHeap _geometryBufferHeap;

		DescriptorHeap _rendaerTargetHeap;

		std::unordered_map<String, ShaderResource> _geometryBufferResource;

		std::unordered_map<String, DXGI_FORMAT> _geometryBufferFormat;

	public:

		GeometryBuffer();
	
		~GeometryBuffer();
	
		void Initialize(unsigned int windowWidth,unsigned int windowHeight);

		void Discard();


	private:

		void InitializeHeap();
		
		void InitializeResource(unsigned int windowWidth, unsigned int windowHeight);
	
	};

}