#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace K3D {
	class DescriptorHeap
	{

	public:

	private:

		//�ICPU�A�h���X�J�n�n�_
		D3D12_CPU_DESCRIPTOR_HANDLE						_cpuHandle;

		//�ICPU�A�h���X�J�n�n�_
		D3D12_GPU_DESCRIPTOR_HANDLE						_gpuHandle;

		//!�^�C�v
		UINT											_type;

		//�I�C���N�������g�T�C�Y
		UINT											_incrementSize;

		//�I�q�[�v�f�X�N���v�V����
		D3D12_DESCRIPTOR_HEAP_DESC						_heapDesc;

		//!�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>	_heap;

		//�I�q�[�v��
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
