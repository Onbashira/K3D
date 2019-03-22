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

		//デバイスの参照
		std::shared_ptr<D3D12Device> _device;

		//ヒープタイプをキー値としたデスクリプタヒープ
		DescHeapMap _heaps;

		//各ヒープタイプにバインドされているviewのタイプ
		ViewMap _allocatedViewMap;

		DescMap _allocatedDescMap;

		//cpuHeapにバインドされたデスクリプタのオフセット　空きブロック検索コストの削減用
		unsigned int _cpuOffset;
		//rtvHeapにバインドされたデスクリプタのオフセット　空きブロック検索コストの削減用
		unsigned int _rtvOffset;
		//dsvHeapにバインドされたデスクリプタのオフセット　空きブロック検索コストの削減用
		unsigned int _dsvOffset;
		//sampelrHeapにバインドされたデスクリプタのオフセット　空きブロック検索コストの削減用
		unsigned int _samplerOffset;

		//各ミューテックス
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

		//動作としては基本的に上書き
		std::weak_ptr<K3D::Descriptor> CreateView(const K3D::GameHeap::HeapType& heapType, const K3D::Descriptor::ViewType& viewType, void* viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource = nullptr);

		//以下の関数は基本的に単一デスクリプタの生成
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