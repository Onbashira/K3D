#include "stdafx.h"
#include "ParticleBinary.h"
#include "Engine/Source/Resource/Resource.h"


K3D::ParticleBinary::ParticleBinary()
{
}


K3D::ParticleBinary::~ParticleBinary()
{
}

HRESULT K3D::ParticleBinary::Initialize(std::shared_ptr<D3D12Device>& device, unsigned int ptBinSize, unsigned int ptHeadersSize, unsigned ptIdxListSize)
{
	HRESULT hr = {};

	hr = PtBinInitialize(device ,ptBinSize);
	CHECK_RESULT(hr);
	hr = PtHeaderesBinInitialize(device, ptHeadersSize);
	CHECK_RESULT(hr);
	hr = PtIdxListBinInitialize(device, ptIdxListSize);
	CHECK_RESULT(hr);
	return hr;
}

void K3D::ParticleBinary::Discard()
{

	this->_particleBin->Discard();
	this->_particleHeadersBin->Discard();
	this->_particleIndexListBin->Discard();

}

HRESULT K3D::ParticleBinary::PtBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned int ptBinSize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = ptBinSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _particleBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);

	return hr;
}

HRESULT K3D::ParticleBinary::PtHeaderesBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned int ptHeadersSize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = ptHeadersSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _particleHeadersBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);

	return hr;
}

HRESULT K3D::ParticleBinary::PtIdxListBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned ptIdxListSize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_L1;;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = ptIdxListSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _particleIndexListBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);

	return hr;
}
