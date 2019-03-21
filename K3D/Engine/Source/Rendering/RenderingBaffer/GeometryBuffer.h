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

		std::unordered_map<String, ShaderResource> _geometryBufferResource;

		std::unordered_map<String, DXGI_FORMAT> _geometryBufferFormat;

	public:

		GeometryBuffer();
	
		~GeometryBuffer();
	
	private:

	};

}