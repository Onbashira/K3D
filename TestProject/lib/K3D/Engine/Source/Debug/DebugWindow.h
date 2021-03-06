#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/ThirdParty/imgui/imgui.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine//Source/Resource/IndexBuffer.h"
#include "Engine/Source/Resource/VertexBuffer.h"
#include <memory>

namespace K3D {
	class D3D12Device;
	class DescriptorHeap;
	class CommandList;

	class DebugWindow
	{
	public:
	private:
		DescriptorHeap _fontHeap;
	public:

		DebugWindow();

		~DebugWindow();

		void Init(std::shared_ptr<D3D12Device>& dev, int frame, DXGI_FORMAT format, D3D12_CPU_DESCRIPTOR_HANDLE fontSrvCPUDescHandle, D3D12_GPU_DESCRIPTOR_HANDLE fontSrvGPUDescHandle);
		
		void ShutDown();

		void NewFrame();

		void RenderData(ImDrawData* drawData, std::weak_ptr<CommandList> list);

		void InvalidDataDevicePbject();

		void CreateDeviceObject();


	private:
	};

}