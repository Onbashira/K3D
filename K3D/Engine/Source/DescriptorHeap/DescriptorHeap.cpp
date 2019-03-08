#include "DescriptorHeap.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include <string>

K3D::DescriptorHeap::DescriptorHeap():
	_heap(), _incrementSize(0), _gpuHandle({}), _cpuHandle({}), _heapName("UnNamed Heap ")
{
	_gpuHandle.ptr = 0;
	_cpuHandle.ptr = 0;
}


K3D::DescriptorHeap::~DescriptorHeap()
{

	Discard();
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D::DescriptorHeap::GetCPUHandle(UINT indexCount)const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (_incrementSize* indexCount);
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D::DescriptorHeap::GetGPUHandle(UINT indexCount)const
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (_incrementSize* indexCount);
	return handle;
}

UINT K3D::DescriptorHeap::GetIncrementSize()const
{
	return _incrementSize;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& K3D::DescriptorHeap::GetHeap()
{
	return _heap;
}

ID3D12DescriptorHeap*  K3D::DescriptorHeap::GetPtr()const
{
	return _heap.Get();
}

const D3D12_DESCRIPTOR_HEAP_DESC & K3D::DescriptorHeap::GetDesc()const
{
	return _heapDesc;
}

ID3D12DescriptorHeap* K3D::DescriptorHeap::operator->()
{
	return _heap.Get();
}

HRESULT K3D::DescriptorHeap::Initialize(D3D12_DESCRIPTOR_HEAP_DESC* desc)
{
	if (desc == nullptr) {
		return E_FAIL;
	}
	auto hr = Framework::GetInstance().GetDevice()->GetDevice()->CreateDescriptorHeap(desc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr)) {
		return  E_FAIL;
	}
	_cpuHandle = this->_heap->GetCPUDescriptorHandleForHeapStart();
	_gpuHandle = this->_heap->GetGPUDescriptorHandleForHeapStart();
	_heapDesc = *desc;
	_type = desc->Type;
	_incrementSize = Framework::GetInstance().GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(desc->Type);
	return S_OK;
}

HRESULT K3D::DescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_DESC * pDesc)
{
	if (pDesc == nullptr) {
		return E_FAIL;
	}
	auto hr = device->GetDevice()->CreateDescriptorHeap(pDesc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr)) {
		return  E_FAIL;
	}
	_cpuHandle = this->_heap->GetCPUDescriptorHandleForHeapStart();
	_gpuHandle = this->_heap->GetGPUDescriptorHandleForHeapStart();
	_heapDesc = *pDesc;
	_type = pDesc->Type;
	_incrementSize = device->GetDevice()->GetDescriptorHandleIncrementSize(pDesc->Type);
	return S_OK;
}

HRESULT K3D::DescriptorHeap::Initialize(D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		break;
	default:
		break;
	}

	auto hr = Initialize(&desc);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D::DescriptorHeap::Initialize(std::shared_ptr<D3D12Device>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int numDescriptors, unsigned int nodeMask)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	switch (type)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		break;
	case D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES:
		desc.NumDescriptors = numDescriptors;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = nodeMask;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		break;
	default:
		break;
	}

	auto hr = Initialize(device,&desc);
	CHECK_RESULT(hr);
	return hr;
}

void K3D::DescriptorHeap::SetName(std::string name)
{
	_heap->SetName(Util::StringToWString(name).c_str());
	_heapName = name;
}

void K3D::DescriptorHeap::Discard()
{
	if (_heap.Get() != nullptr) {
		_heap.Reset();
		_gpuHandle.ptr = 0;
		_cpuHandle.ptr = 0;
		_incrementSize = 0;
		DEBUG_LOG(std::string("DescriptorHeap : " + _heapName + " is Reset"));

	}
}
