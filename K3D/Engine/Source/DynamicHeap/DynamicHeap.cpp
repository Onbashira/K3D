#include "DynamicHeap.h"
#include "Engine/Source/Utility/BasicTypes.h"
#include "Engine/Source/Utility/D3D12Common.h"

K3D::DynamicPage::DynamicPage(ID3D12Device * dev, UINT64 size)
{
	D3D12_HEAP_PROPERTIES prop = {};
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment = 0;
	resDesc.Width = size;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES defaultUsage = D3D12_RESOURCE_STATE_GENERIC_READ;

	_resource = std::make_shared<Resource>();
	_resource->Create(prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, resDesc, defaultUsage);

	_resource->SetName(" Dynamic Memory Page ");

	_gpuVirtualAddress = _resource->GetResource()->GetGPUVirtualAddress();
	_resource->Map(0, nullptr);
	_cpuVirtualAddress = reinterpret_cast<void*>(_resource->GetMappedPointer());

}


K3D::DynamicMemoryManager::DynamicMemoryManager(IMemoryAllocator & allocator, ID3D12Device & device, Uint32 numPagesToReserve, UINT64 pageSize) :
	_dev(&device),
	_availablePages(STD_ALLOCATOR_RAW_MEM(AvailablePagesMapElemType, allocator, "Allocator for multimap<AvailablePagesMapElemType>"))
{
	for (Uint32 i = 0; i < numPagesToReserve; ++i) {

		DynamicPage Page(_dev, pageSize);
		
		auto size = Page.GetSize();
		
		_availablePages.emplace(size, std::move(Page));

	}

}

K3D::DynamicMemoryManager::~DynamicMemoryManager()
{

}

void K3D::DynamicMemoryManager::ReleasePages(std::vector<DynamicPage>& pages, Uint64 QueueMask)
{
	struct StalePage {
		DynamicPage Page;
		DynamicMemoryManager* Mgr;
		StalePage(DynamicPage& page, DynamicMemoryManager& mgr)noexcept :
			Page(std::move(page)), Mgr(&mgr)
		{
		}
		StalePage(const StalePage&) = delete;
		StalePage& operator= (const StalePage&) = delete;
		StalePage& operator= (StalePage&&) = delete;

		StalePage(StalePage&& rhs)noexcept :
			Page(std::move(rhs.Page)),
			Mgr(rhs.Mgr)
		{
			rhs.Mgr = nullptr;
		}

		~StalePage()
		{
			if (Mgr != nullptr)
			{
				std::lock_guard<std::mutex> Lock(Mgr->_availablePageMtx);
				auto PageSize = Page.GetSize();
				Mgr->_availablePages.emplace(PageSize, std::move(Page));
			}
		}
	};
	//for (auto& Page : pages)
	//{
	//	m_DeviceD3D12Impl.SafeReleaseDeviceObject(StalePage{ std::move(Page), *this }, QueueMask);
	//}
}

void K3D::DynamicMemoryManager::Destroy()
{
	Uint64 TotalAllocatedSize = 0;
	for (const auto& Page : _availablePages)
		TotalAllocatedSize += Page.second.GetSize();

	_availablePages.clear();
}

K3D::DynamicPage K3D::DynamicMemoryManager::AllocatePage(Uint64 sizeInBytes)
{
	std::lock_guard<std::mutex> AvailablePagesLock(_availablePageMtx);
//#ifdef DEVELOPMENT
//	++m_AllocatedPageCounter;
//#endif
	auto PageIt = _availablePages.lower_bound(sizeInBytes); // Returns an iterator pointing to the first element that is not less than key
	if (PageIt != _availablePages.end())
	{
		//VERIFY_EXPR(PageIt->first >= SizeInBytes);
		DynamicPage Page(std::move(PageIt->second));
		_availablePages.erase(PageIt);
		return Page;
	}
	else
	{
		return DynamicPage{ _dev, sizeInBytes };
	}
}

K3D::DynamicHeap::DynamicHeap(DynamicMemoryManager & dynamicMemMgr, std::string heapName, Uint64 pageSize) :
	_globalDynamicMemMgr(dynamicMemMgr),_heapName(std::move(heapName)),_pageSize(pageSize)
{
}

K3D::DynamicHeap::~DynamicHeap()
{
}

K3D::DynamicAllocation K3D::DynamicHeap::Allocate(Uint64 sizeInBytes, Uint64 alignment, Uint64 DvpCtxFrameNumber)
{	
	
	if (_currentOffset == InvalidOffset || sizeInBytes + (Util::Align(_currentOffset,alignment) - _currentOffset) > _availableSize) 
	{
		auto newPageSize = _pageSize;
		while (newPageSize < sizeInBytes) {
			newPageSize *= 2;
		}
		auto newPage = _globalDynamicMemMgr.AllocatePage(newPageSize);
		if (newPage.IsValid()) {
			_currentOffset = 0;
			_availableSize = newPage.GetSize();

			_currentAllocatedSize += _availableSize;
			_peakAllocatedSize = max(_peakAllocatedSize, _currentAllocatedSize);

			_allocatedPages.emplace_back(std::move(newPage));
		}
	}

	if (_currentOffset != InvalidOffset && sizeInBytes + (Util::Align(_currentOffset, alignment) - _currentOffset) <= _availableSize)
	{
		auto AlignedOffset = Util::Align(_currentOffset, alignment);
		auto AdjustedSize = sizeInBytes + (AlignedOffset - _currentOffset);
		//VERIFY_EXPR(AdjustedSize <= m_AvailableSize);
		_availableSize -= AdjustedSize;
		_currentOffset += AdjustedSize;

		_currentUsedSize += sizeInBytes;
		_peakUsedSize = max(_peakUsedSize, _currentUsedSize);

		_currentAlignedSize += AdjustedSize;
		_peakAlignedSize = max(_peakAlignedSize, _currentAlignedSize);

		auto& CurrPage = _allocatedPages.back();
		return DynamicAllocation
		(
			CurrPage.GetResource().lock(),
			AlignedOffset,
			sizeInBytes,
			CurrPage.GetCPUAddress(AlignedOffset),
			CurrPage.GetGPUAddress(AlignedOffset)
//#ifdef DEVELOPMENT
//		  , DvpCtxFrameNumber
//#endif
		);
	}
	return DynamicAllocation();
}

void K3D::DynamicHeap::ReleaseAllocatedPages(Uint64 queueMask)
{
	_globalDynamicMemMgr.ReleasePages(_allocatedPages, queueMask);
	_allocatedPages.clear();

	_currentOffset = InvalidOffset;
	_availableSize = 0;
	_currentAllocatedSize = 0;
	_currentUsedSize = 0;
	_currentAlignedSize = 0;
}
