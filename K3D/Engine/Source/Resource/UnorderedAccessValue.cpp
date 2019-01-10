#include "UnorderedAccessValue.h"
#include "Engine/Source/CoreSystem/Framework.h"


K3D::UnorderedAccessValue::UnorderedAccessValue()
{
}


K3D::UnorderedAccessValue::~UnorderedAccessValue()
{
	this->Discard();
}

HRESULT K3D::UnorderedAccessValue::CreateHeap(unsigned numElements, unsigned int nodeMask)
{
	auto hr = this->_heap.Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, numElements, nodeMask);
	_heap.SetName("UnorderedAccessValue_Heap");
	return hr;
}

HRESULT K3D::UnorderedAccessValue::Create(unsigned int elementSize, unsigned int numElements, void* pBufferData)
{
	if (elementSize == 0 || numElements == 0) {
		return E_FAIL;
	}
#pragma region Initilaize UnorderdResource and StagingResource


	{
		D3D12_HEAP_PROPERTIES defaultHeapProp = {};
		D3D12_HEAP_PROPERTIES uploadHeapProp = {};

		D3D12_RESOURCE_DESC defaultResourceDesc;
		D3D12_RESOURCE_DESC uploadResourceDesc;
		Resource uploadResource;

		{
			defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
			defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
			defaultHeapProp.VisibleNodeMask = 1;
			defaultHeapProp.CreationNodeMask = 1;
			defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;
			{
			//	defaultHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
			//	defaultHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			//	defaultHeapProp.VisibleNodeMask = 1;
			//	defaultHeapProp.CreationNodeMask = 1;
			//	defaultHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
			}
		}

		{

			uploadHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_READBACK;
			uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			uploadHeapProp.VisibleNodeMask = 1;
			uploadHeapProp.CreationNodeMask = 1;
			uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		}

		{
			//defaultResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numElements * elementSize, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
			defaultResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
			defaultResourceDesc.Width = numElements * elementSize;
			defaultResourceDesc.Height = 1;
			defaultResourceDesc.DepthOrArraySize = 1;
			defaultResourceDesc.MipLevels = 0;
			defaultResourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
			defaultResourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
			defaultResourceDesc.SampleDesc.Count = 1;
			defaultResourceDesc.SampleDesc.Quality = 0;
			defaultResourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		}
		{
			uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numElements * elementSize);
		}

		//リソース作成
		{

			_stagingResource.Create(defaultHeapProp, D3D12_HEAP_FLAG_NONE, defaultResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
			Resource::Create(uploadHeapProp, D3D12_HEAP_FLAG_NONE, uploadResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
			
			uploadHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
			_uploadResource.Create(uploadHeapProp, D3D12_HEAP_FLAG_NONE, uploadResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
#ifdef _DEBUG
			SetName("UAVUploadResource");
			_stagingResource.SetName("UAVStagingResource");
#endif // _DEBUG

		}
	}
#pragma endregion
	//ReadRangeSetUp
	{
		_readRange.Begin = 0;
		_readRange.End = numElements;
	}

	CHECK_RESULT(CreateHeap(HEAP_OFFSET::HEAP_OFFSET_MAX));
	//Setting View
	{
		D3D12_BUFFER_UAV uavBuffer;
		uavBuffer.FirstElement = 0;
		uavBuffer.NumElements = numElements;
		uavBuffer.Flags = D3D12_BUFFER_UAV_FLAGS::D3D12_BUFFER_UAV_FLAG_NONE;
		uavBuffer.CounterOffsetInBytes = 0;
		uavBuffer.StructureByteStride = elementSize;


		this->_unorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
		_unorderedAccessViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		_unorderedAccessViewDesc.Buffer = uavBuffer;

		_shaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		_shaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		_shaderResourceViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
		_shaderResourceViewDesc.Buffer.FirstElement = 0;
		_shaderResourceViewDesc.Buffer.NumElements = numElements;
		_shaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;
		_shaderResourceViewDesc.Buffer.StructureByteStride = elementSize;
	}
	CHECK_RESULT(CreateDescriptors(elementSize, numElements));

	return S_OK;
}

HRESULT K3D::UnorderedAccessValue::CreateDescriptors(unsigned int elementSize, unsigned int numElements)
{
	//UAVStructuredView
	{
		this->CreateView(&_unorderedAccessViewDesc, _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET));
	}
	//SRVRawView
	{
		this->CreateView(&_shaderResourceViewDesc, _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET));
	}
	return S_OK;

}

D3D12_CPU_DESCRIPTOR_HANDLE K3D::UnorderedAccessValue::GetSRVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_CPU_DESCRIPTOR_HANDLE K3D::UnorderedAccessValue::GetUAVCPUHandle()
{
	return _heap.GetCPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D::UnorderedAccessValue::GetSRVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::SRV_DESCRIPTOR_OFFSET);
}

D3D12_GPU_DESCRIPTOR_HANDLE K3D::UnorderedAccessValue::GetUAVGPUHandle()
{
	return _heap.GetGPUHandle(HEAP_OFFSET::UAV_DESCRIPTOR_OFFSET);
}

HRESULT K3D::UnorderedAccessValue::CreateView(D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	Framework::GetDevice().GetDevice()->CreateUnorderedAccessView(_stagingResource.GetResource(), nullptr, uavDesc, cpuDescriptorHandle);
	return S_OK;
}

HRESULT K3D::UnorderedAccessValue::CreateView(D3D12_SHADER_RESOURCE_VIEW_DESC * srvDesc, D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandle)
{
	Framework::GetDevice().GetDevice()->CreateShaderResourceView(_stagingResource.GetResource(), srvDesc, cpuDescriptorHandle);
	return S_OK;
}

void K3D::UnorderedAccessValue::AsyncWriteToBuffer(std::weak_ptr<K3D::CommandList> list, unsigned int numElements, unsigned int elementSize, void * pBufferData, K3D::CommandQueue* queue)
{

	if (pBufferData == nullptr) {
		return;
	}

	assert(!list.expired());

	D3D12_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pData = pBufferData;
	subresourceData.RowPitch = numElements * elementSize;
	subresourceData.SlicePitch = 1;
	D3D12_RESOURCE_DESC uploadResourceDesc;
	D3D12_HEAP_PROPERTIES uploadHeapProp = {};

	{
		uploadHeapProp.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		uploadHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		uploadHeapProp.VisibleNodeMask = 1;
		uploadHeapProp.CreationNodeMask = 1;
		uploadHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	}
	{
		uploadResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(numElements * elementSize);
	}
	_uploadResource.Discard();
	_uploadResource.Create(uploadHeapProp, D3D12_HEAP_FLAG_NONE, uploadResourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);


	//ステージングのためのリソース遷移
	_stagingResource.ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	//リソース送信
	UpdateSubresources<1>(list.lock()->GetCommandList().Get(), _stagingResource.GetResource(), _uploadResource.GetResource(), 0, 0, 1, &subresourceData);
	_stagingResource.ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void K3D::UnorderedAccessValue::AsyncReadBack(std::weak_ptr<K3D::CommandList> list, K3D::CommandQueue* queue)
{
	assert(!list.expired());
	_stagingResource.ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);
	list.lock()->GetCommandList()->CopyResource(this->_resource.Get(), _stagingResource.GetResource());
	_stagingResource.ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
}

void K3D::UnorderedAccessValue::Discard()
{
	_heap.Discard();
	_stagingResource.Discard();
}

K3D::DescriptorHeap * K3D::UnorderedAccessValue::GetHeap()
{
	return &_heap;
}

D3D12_UNORDERED_ACCESS_VIEW_DESC K3D::UnorderedAccessValue::GetUAVDesc()
{
	return _unorderedAccessViewDesc;

}

D3D12_SHADER_RESOURCE_VIEW_DESC K3D::UnorderedAccessValue::GetSRVDesc()
{
	return _shaderResourceViewDesc;
}
