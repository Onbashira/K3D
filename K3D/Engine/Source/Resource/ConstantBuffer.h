#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Resource.h"

namespace K3D {

	class DescriptorHeap;

	class ConstantBuffer : public Resource
	{

	public:

	private:

	public:
				
		ConstantBuffer();
		
		~ConstantBuffer();

		virtual HRESULT Create(ULONG64 size);
		
		HRESULT			CreateView(D3D12_CONSTANT_BUFFER_VIEW_DESC& cvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle);

	private:

	};
}