#pragma once
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Utility/BasicTypes.h"
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/MemoryAllocator/STDAllocator.h"
#include "Engine/Source/MemoryAllocator/DefaultRawMemoryAllocator.h"
#include <d3d12.h>
#include <map>
#include <memory>
#include <mutex>

namespace K3D {

	class Resource;

	struct DynamicAllocation
	{
		std::shared_ptr<Resource> resource;

		Uint64 offset = 0;

		Uint64 size = 0;

		void* cpuAddress = nullptr;

		D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = 0;

		DynamicAllocation()noexcept {};

		DynamicAllocation(std::shared_ptr<Resource> pBuffer, K3D::Uint64 thisOffset, K3D::Uint64 thisSize, void* cpuAddress, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress) noexcept :
			resource(pBuffer), offset(thisOffset), size(thisSize), cpuAddress(cpuAddress), gpuAddress(gpuAddress)
		{
		}

		~DynamicAllocation() {};
	};

	class DynamicPage
	{
	public:
	private:

		std::shared_ptr<Resource> _resource;

		void* _cpuVirtualAddress = nullptr;

		D3D12_GPU_VIRTUAL_ADDRESS _gpuVirtualAddress = 0;

	public:
		DynamicPage(ID3D12Device* dev, UINT64 size);

		DynamicPage(const DynamicPage&) = delete;
		DynamicPage(DynamicPage&&) = default;
		DynamicPage& operator= (const DynamicPage&) = delete;
		DynamicPage& operator= (DynamicPage&&) = delete;

		void* GetCPUAddress(UINT64 offset)
		{
			if (offset > GetSize()) {
				return nullptr;
			}
			return reinterpret_cast<Uint8*>(_cpuVirtualAddress) + offset;
		};

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress(UINT64 offset)
		{
			if (offset > GetSize()) {
				return 0;
			}
			return _gpuVirtualAddress + offset;
		}

		std::weak_ptr<Resource> GetResource() {
			return _resource;
		}

		UINT64 GetSize()const {
			if (IsValid()) {
				return 0;
			}
			return _resource->GetResourceDesc()->Width;
		}

		bool IsValid()const { return _resource != nullptr; };

	private:

	};

	class DynamicMemoryManager
	{
	public:
	private:

		ID3D12Device* _dev;

		std::mutex _availablePageMtx;

		using AvailablePagesMapElemType = std::pair<Uint64, DynamicPage>;

		std::multimap<Uint64, DynamicPage, std::less<Uint64>, STDAllocatorRawMem<std::pair<Uint64, DynamicPage>>> _availablePages;


	public:

		DynamicMemoryManager(IMemoryAllocator& allocator, ID3D12Device& device, Uint32 numPagesToReserve, UINT64 pageSize);
		
		~DynamicMemoryManager();
		
		DynamicMemoryManager(const DynamicMemoryManager&) = delete;

		DynamicMemoryManager(DynamicMemoryManager&&) = delete;

		DynamicMemoryManager& operator= (const DynamicMemoryManager&) = delete;

		DynamicMemoryManager& operator= (DynamicMemoryManager&&) = delete;

		void ReleasePages(std::vector<DynamicPage>& pages, Uint64 QueueMask);

		void Destroy();

		DynamicPage AllocatePage(Uint64 sizeInBytes);

	private:

	};

	class DynamicHeap
	{
	public:
	private:

		static constexpr Uint64 InvalidOffset = static_cast<Uint64>(-1);

		DynamicMemoryManager& _globalDynamicMemMgr;
		const std::string _heapName;

		std::vector<DynamicPage> _allocatedPages;

		const Uint64 _pageSize;

		Uint64 _currentOffset = InvalidOffset;

		Uint64 _availableSize = 0;

		Uint64 _currentAllocatedSize = 0;

		Uint64 _currentUsedSize = 0;

		Uint64 _currentAlignedSize = 0;

		Uint64 _peakAllocatedSize = 0;

		Uint64 _peakUsedSize = 0;

		Uint64 _peakAlignedSize = 0;

	public:

		DynamicHeap(DynamicMemoryManager& dynamicMemMgr, std::string heapName, Uint64 pageSize);

		~DynamicHeap();

		DynamicHeap(const DynamicHeap&) = delete;

		DynamicHeap(DynamicHeap&&) = delete;

		DynamicHeap& operator= (const DynamicHeap&) = delete;

		DynamicHeap& operator= (DynamicHeap&&) = delete;

		DynamicAllocation Allocate(Uint64 sizeInBytes, Uint64 alignment, Uint64 DvpCtxFrameNumber);

		void ReleaseAllocatedPages(Uint64 queueMask);

		size_t GetAllocatedPageCount()const { return _allocatedPages.size(); };

	private:


	};

	template<class T> using STDDeleterRawMem = STDDeleter<T, IMemoryAllocator>;


}