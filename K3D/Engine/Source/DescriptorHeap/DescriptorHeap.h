#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace K3D {
	class DescriptorHeap
	{

	public:

	private:

		//！CPUアドレス開始地点
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuHandle;

		//！CPUアドレス開始地点
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuHandle;

		//!タイプ
		UINT											_type;

		//！インクリメントサイズ
		UINT											_incrementSize;

		//！ヒープデスクリプション
		D3D12_DESCRIPTOR_HEAP_DESC						_heapDesc;

		//!ヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;

		//！ヒープ名
		std::string										_heapName;

	public:
		DescriptorHeap();

		~DescriptorHeap();


		D3D12_CPU_DESCRIPTOR_HANDLE						GetCPUHandle(UINT indexCount)const;

		D3D12_GPU_DESCRIPTOR_HANDLE						GetGPUHandle(UINT indexCount)const;

		UINT											GetIncrementSize()const;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	GetHeap()const;

		ID3D12DescriptorHeap* 							GetPtr()const;

		const D3D12_DESCRIPTOR_HEAP_DESC&				GetDesc()const;

		ID3D12DescriptorHeap*							operator ->();

		HRESULT											Create(D3D12_DESCRIPTOR_HEAP_DESC* pDesc);

		HRESULT											Create(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask = 0);

		void											SetName(std::string name);

		void											Discard();

	private:
	};

}
