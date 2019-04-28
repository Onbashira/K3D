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
		std::unordered_map<unsigned char, ShaderResource> _layeredDecalResoruce;
	private:

	public:

		DecaleBuffer();

		~DecaleBuffer();

	private:

	};

}