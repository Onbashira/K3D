#include "stdafx.h"
#include "GameHeap.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Resource/Resource.h"

K3D::GameHeap::DescHeapMap;
K3D::GameHeap::ViewMap ;

K3D::GameHeap::GameHeap(unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize) :
	_cpuOffset(0), _rtvOffset(0), _dsvOffset(0), _samplerOffset(0)
{
	_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::CPU]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, maxCPUHeapSize);

	_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::RTV]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, maxRTHeapSize);

	_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::DSV]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, maxDSHeapSize);

	_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::SAMP]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, maxSampHeapSize);

	_device = K3D::Framework::GetInstance().GetDevice();
}

K3D::GameHeap::GameHeap(std::shared_ptr<D3D12Device>& device, unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize) :
	_device(device), _cpuOffset(0), _rtvOffset(0), _dsvOffset(0), _samplerOffset(0)
{
	_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::CPU]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, maxCPUHeapSize);

	_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::RTV]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, maxRTHeapSize);

	_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::DSV]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, maxDSHeapSize);

	_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::SAMP]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, maxSampHeapSize);
}

K3D::GameHeap::~GameHeap()
{
	Discard();
	_device.reset();
}

HRESULT K3D::GameHeap::ReInitialize(unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize)
{
	Discard();
	_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::CPU]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, maxCPUHeapSize);

	_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::RTV]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, maxRTHeapSize);

	_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::DSV]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, maxDSHeapSize);

	_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
	_heaps[HeapType::SAMP]->Create(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, maxSampHeapSize);

	return S_OK;
}

HRESULT K3D::GameHeap::ReCreateHeap(const HeapType & heapType, unsigned int maxHeapSize)
{
	switch (heapType)
	{
	case K3D::GameHeap::HeapType::CPU:
	{
		unsigned int size = _heaps[HeapType::CPU]->GetDesc().NumDescriptors;
		_heaps[HeapType::CPU].reset();
		_heaps[HeapType::CPU] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::CPU]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, size);
	}
	break;
	case K3D::GameHeap::HeapType::RTV:
	{
		unsigned int size = _heaps[HeapType::RTV]->GetDesc().NumDescriptors;
		_heaps[HeapType::RTV].reset();
		_heaps[HeapType::RTV] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::RTV]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV, size);
	}
	break;
	case K3D::GameHeap::HeapType::DSV:
	{
		unsigned int size = _heaps[HeapType::DSV]->GetDesc().NumDescriptors;
		_heaps[HeapType::DSV].reset();
		_heaps[HeapType::DSV] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::DSV]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV, size);
	}
	break;
	case K3D::GameHeap::HeapType::SAMP:
	{
		unsigned int size = _heaps[HeapType::SAMP]->GetDesc().NumDescriptors;
		_heaps[HeapType::SAMP].reset();
		_heaps[HeapType::SAMP] = std::make_shared<DescriptorHeap>();
		_heaps[HeapType::SAMP]->Create(_device, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, size);
	}
	break;
	default:
		break;
	}
	return S_OK;
}

K3D::GameHeap::Descriptor K3D::GameHeap::GetDescriptorHandle(const K3D::GameHeap::HeapType & heapType, unsigned int handleOffset)
{

	switch (heapType)
	{
	case K3D::GameHeap::HeapType::CPU:
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::CPU]->GetCPUHandle(handleOffset);
		descriptor.gpuAddress = _heaps[HeapType::CPU]->GetGPUHandle(handleOffset);
		descriptor.heapType = K3D::GameHeap::HeapType::CPU;
		descriptor.offset = handleOffset;
		descriptor.viewType = this->_allocatedViewMap[heapType][handleOffset];
		return descriptor;
	}
	break;
	case K3D::GameHeap::HeapType::RTV:
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::RTV]->GetCPUHandle(handleOffset);
		descriptor.gpuAddress = _heaps[HeapType::RTV]->GetGPUHandle(handleOffset);
		descriptor.heapType = K3D::GameHeap::HeapType::RTV;
		descriptor.offset = handleOffset;
		descriptor.viewType = K3D::GameHeap::ViewType::RTV;
		return descriptor;
	}
	break;
	case K3D::GameHeap::HeapType::DSV:
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::DSV]->GetCPUHandle(handleOffset);
		descriptor.gpuAddress = _heaps[HeapType::DSV]->GetGPUHandle(handleOffset);
		descriptor.heapType = K3D::GameHeap::HeapType::DSV;
		descriptor.offset = handleOffset;
		descriptor.viewType = K3D::GameHeap::ViewType::DSV;
		return descriptor;
	}
	break;
	case K3D::GameHeap::HeapType::SAMP:
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::SAMP]->GetCPUHandle(handleOffset);
		descriptor.gpuAddress = _heaps[HeapType::SAMP]->GetGPUHandle(handleOffset);
		descriptor.heapType = K3D::GameHeap::HeapType::SAMP;
		descriptor.offset = handleOffset;
		descriptor.viewType = K3D::GameHeap::ViewType::SAMP;
		return descriptor;
	}
	break;
	default:
		break;
	}
	return K3D::GameHeap::Descriptor();
}

K3D::GameHeap::Descriptor K3D::GameHeap::CreateView(const K3D::GameHeap::HeapType & heapType, const K3D::GameHeap::ViewType & viewType, void * viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource)
{
	switch (viewType)
	{
	case K3D::GameHeap::ViewType::CBV:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateConstantBufferView((D3D12_CONSTANT_BUFFER_VIEW_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;

			_allocatedViewMap[heapType][handleOffset] = viewType;
			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::SRV:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateShaderResourceView(resource->GetResource().Get(), (D3D12_SHADER_RESOURCE_VIEW_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;
			_allocatedViewMap[heapType][handleOffset] = viewType;
			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::UAV:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateUnorderedAccessView(resource->GetResource().Get(), counterResource->GetResource().Get(), (D3D12_UNORDERED_ACCESS_VIEW_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;
			_allocatedViewMap[heapType][handleOffset] = viewType;
			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::RTV:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateRenderTargetView(resource->GetResource().Get(), (D3D12_RENDER_TARGET_VIEW_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;

			_allocatedViewMap[heapType][handleOffset] = viewType;
			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::DSV:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateDepthStencilView(resource->GetResource().Get(), (D3D12_DEPTH_STENCIL_VIEW_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;
			_allocatedViewMap[heapType][handleOffset] = viewType;

			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::SAMP:
		auto cpuPtr = _heaps[heapType]->GetCPUHandle(handleOffset);
		_device->GetDevice()->CreateSampler((D3D12_SAMPLER_DESC*)viewDesc, cpuPtr);
		{
			Descriptor descriptor;
			descriptor.cpuHandle = _heaps[heapType]->GetCPUHandle(handleOffset);
			descriptor.gpuAddress = _heaps[heapType]->GetGPUHandle(handleOffset);
			descriptor.heapType = heapType;
			descriptor.offset = handleOffset;
			descriptor.viewType = viewType;

			_allocatedViewMap[heapType][handleOffset] = viewType;

			return descriptor;
		}
		break;
	case K3D::GameHeap::ViewType::EMPTY:
	{
		Descriptor descriptor;
		descriptor.cpuHandle.ptr = 0;
		descriptor.gpuAddress.ptr = 0;
		descriptor.heapType = HeapType::EMPTY;
		descriptor.offset = 0;
		descriptor.viewType = ViewType::EMPTY;
		return descriptor;
	}
	break;
	default:
		break;
	}
}

K3D::GameHeap::Descriptor K3D::GameHeap::CreateCBView(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc)
{
	std::lock_guard mutex(_cpuMutex);

	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateConstantBufferView(&desc, cpuPtr);

	Descriptor descriptor;
	descriptor.cpuHandle = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	descriptor.gpuAddress = _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset);
	descriptor.heapType = HeapType::CPU;
	descriptor.offset = _cpuOffset;
	descriptor.viewType = ViewType::CBV;

	_allocatedViewMap[HeapType::CPU][_cpuOffset++] = ViewType::CBV;

	return descriptor;
}

K3D::GameHeap::Descriptor K3D::GameHeap::CreateSRView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc,  Resource * resource)
{
	std::lock_guard mutex(_cpuMutex);

	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateShaderResourceView(resource->GetResource().Get(), &desc, cpuPtr);
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
		descriptor.gpuAddress = _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset);
		descriptor.heapType = HeapType::CPU;
		descriptor.offset = _cpuOffset;
		descriptor.viewType = ViewType::SRV;

		_allocatedViewMap[HeapType::CPU][_cpuOffset++] = ViewType::SRV;

		return descriptor;
	}
}

K3D::GameHeap::Descriptor K3D::GameHeap::CreateUAView(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc,  Resource * resource,  Resource * counterResource)
{
	std::lock_guard mutex(_cpuMutex);


	auto cpuPtr = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
	_device->GetDevice()->CreateUnorderedAccessView(resource->GetResource().Get(), counterResource->GetResource().Get(), &desc, cpuPtr);
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::CPU]->GetCPUHandle(_cpuOffset);
		descriptor.gpuAddress = _heaps[HeapType::CPU]->GetGPUHandle(_cpuOffset);
		descriptor.heapType = HeapType::CPU;
		descriptor.offset = _cpuOffset;
		descriptor.viewType = ViewType::UAV;

		_allocatedViewMap[HeapType::CPU][_cpuOffset++] = ViewType::UAV;

		return descriptor;
	}
}


K3D::GameHeap::Descriptor K3D::GameHeap::CreateRTView(D3D12_RENDER_TARGET_VIEW_DESC& desc, Resource * resource)
{
	std::lock_guard mutex(_rtvMutex);

	auto cpuPtr = _heaps[HeapType::RTV]->GetCPUHandle(_rtvOffset);
	_device->GetDevice()->CreateRenderTargetView(resource->GetResource().Get(), &desc, cpuPtr);

	Descriptor descriptor;
	descriptor.cpuHandle = _heaps[HeapType::RTV]->GetCPUHandle(_rtvOffset);
	descriptor.gpuAddress = _heaps[HeapType::RTV]->GetGPUHandle(_rtvOffset);
	descriptor.heapType = HeapType::RTV;
	descriptor.offset = _rtvOffset;
	descriptor.viewType = ViewType::RTV;

	_allocatedViewMap[HeapType::RTV][_rtvOffset++] = ViewType::RTV;
	return descriptor;

}


K3D::GameHeap::Descriptor K3D::GameHeap::CreateDSView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc,  Resource * resource)
{
	std::lock_guard mutex(_dsvMutex);

	auto cpuPtr = _heaps[HeapType::DSV]->GetCPUHandle(_dsvOffset);
	_device->GetDevice()->CreateDepthStencilView(resource->GetResource().Get(), &desc, cpuPtr);
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::DSV]->GetCPUHandle(_dsvOffset);
		descriptor.gpuAddress = _heaps[HeapType::DSV]->GetGPUHandle(_dsvOffset);
		descriptor.heapType = HeapType::DSV;
		descriptor.offset = _dsvOffset;
		descriptor.viewType = ViewType::DSV;
		_allocatedViewMap[HeapType::DSV][_dsvOffset++] = ViewType::DSV;

		return descriptor;
	}
}

K3D::GameHeap::Descriptor K3D::GameHeap::CreateSampView(D3D12_SAMPLER_DESC & desc)
{
	std::lock_guard mutex(_dsvMutex);

	auto cpuPtr = _heaps[HeapType::SAMP]->GetCPUHandle(_samplerOffset);
	_device->GetDevice()->CreateSampler(&desc, cpuPtr);
	{
		Descriptor descriptor;
		descriptor.cpuHandle = _heaps[HeapType::SAMP]->GetCPUHandle(_samplerOffset);
		descriptor.gpuAddress = _heaps[HeapType::SAMP]->GetGPUHandle(_samplerOffset);
		descriptor.heapType = HeapType::SAMP;
		descriptor.offset = _samplerOffset;
		descriptor.viewType = ViewType::SAMP;
		_allocatedViewMap[HeapType::SAMP][_samplerOffset++] = ViewType::SAMP;

		return descriptor;
	}
}

void K3D::GameHeap::Discard()
{
	_heaps[HeapType::CPU]->Discard();
	_heaps[HeapType::RTV]->Discard();
	_heaps[HeapType::DSV]->Discard();
	_heaps[HeapType::SAMP]->Discard();
	this->_allocatedViewMap[HeapType::CPU].clear();
	this->_allocatedViewMap[HeapType::RTV].clear();
	this->_allocatedViewMap[HeapType::DSV].clear();
	this->_allocatedViewMap[HeapType::SAMP].clear();
	_cpuOffset = 0;
	_rtvOffset = 0;
	_dsvOffset = 0;
	_samplerOffset = 0;

}
