#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/MemoryAllocator/VariableSizeAllocationManager.h"
namespace K3D {

	class Resource;
	class D3D12Device;
	class CommandList;

	class GameHeap
	{
	public:

		enum class HeapType {
			CPU, RTV, DSV, SAMP, EMPTY
		};

		using DescHeapMap = std::map<K3D::GameHeap::HeapType, std::shared_ptr<DescriptorHeap>>;

		using ViewMap = std::map<K3D::GameHeap::HeapType, std::vector<Descriptor::ViewType>>;

		using DescMap = std::map<K3D::GameHeap::HeapType, std::vector<std::shared_ptr<Descriptor>>>;
		


		struct FreeBlocksInfo {

			int head = -1;
			int tail = -1;


		};


	private:

		//�f�o�C�X�̎Q��
		std::shared_ptr<D3D12Device> _device;

		//�q�[�v�^�C�v���L�[�l�Ƃ����f�X�N���v�^�q�[�v
		DescHeapMap _heaps;

		//�e�q�[�v�^�C�v�Ƀo�C���h����Ă���view�̃^�C�v
		ViewMap _allocatedViewMap;

		DescMap _allocatedDescMap;

		//cpuHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _cpuOffset;
		//rtvHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _rtvOffset;
		//dsvHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _dsvOffset;
		//sampelrHeap�Ƀo�C���h���ꂽ�f�X�N���v�^�̃I�t�Z�b�g�@�󂫃u���b�N�����R�X�g�̍팸�p
		unsigned int _samplerOffset;

		//�e�~���[�e�b�N�X
		std::mutex _cpuMutex;
		std::mutex _rtvMutex;
		std::mutex _dsvMutex;
		std::mutex _samplerMutex;

		inline static std::shared_ptr<Descriptor> InvalidDescriptor = std::make_shared<Descriptor>();

	public:

		GameHeap(std::shared_ptr<D3D12Device>& device, unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		~GameHeap();

		static std::shared_ptr<GameHeap> CreateGameHeap(std::shared_ptr<D3D12Device>& device, unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		HRESULT ReInitialize(unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		HRESULT ReCreateHeap(const GameHeap::HeapType& heapType, unsigned int maxHeapSize);

		std::weak_ptr<K3D::Descriptor> GetDescriptorHandle(const K3D::GameHeap::HeapType& heapType, unsigned int handleOffset);

		//����Ƃ��Ă͊�{�I�ɏ㏑��
		std::weak_ptr<K3D::Descriptor> CreateView(const K3D::GameHeap::HeapType& heapType, const K3D::Descriptor::ViewType& viewType, void* viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource = nullptr);

		//�ȉ��̊֐��͊�{�I�ɒP��f�X�N���v�^�̐���
		std::weak_ptr<K3D::Descriptor> CreateCBView(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

		std::weak_ptr<K3D::Descriptor> CreateSRView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<K3D::Descriptor> CreateUAView(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, Resource* resource, Resource* counterResource = nullptr);

		std::weak_ptr<K3D::Descriptor> CreateRTView(D3D12_RENDER_TARGET_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<K3D::Descriptor> CreateDSView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, Resource* resource);

		std::weak_ptr<K3D::Descriptor> CreateSampView(D3D12_SAMPLER_DESC& desc);

		void Discard();

	private:



	};

}