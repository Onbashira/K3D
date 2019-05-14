#include "stdafx.h"
#include "EmitterBinary.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"
#include "Engine/Source/Resource/Resource.h"

K3D::EmitterBinary::EmitterBinary()
{

}


K3D::EmitterBinary::~EmitterBinary()
{
	Discard();
}

HRESULT K3D::EmitterBinary::Initialize(std::shared_ptr<D3D12Device>& device, unsigned int binarySize)
{

	auto hr = EmitterBinInit(device, binarySize);
	CHECK_RESULT(hr);
	hr = EmitterHeaderesBinInit(device, binarySize);
	CHECK_RESULT(hr);
	hr = EmitterTableBinInit(device, binarySize);
	CHECK_RESULT(hr);

	return hr;
}


//EmitterBinary領域にアイテムを書き込む（EmitterHeader部は別領域に書き込み）
void K3D::EmitterBinary::Write(const CPUEmitterHeader* header,std::shared_ptr<Emitter>& emitter)
{
	unsigned offset = header->EmitterBinHead;
	unsigned binSize = emitter->GetBinSize();
	unsigned dataSize = 0;
	//データ確保
	char* ptr = new char[emitter->GetBinSize()];

	//メモリコピー
	for (auto& item : emitter->GetEmitterItems()) {
		dataSize = item->GetElementSize();
		std::memcpy(ptr, item->GetValue(), dataSize);
		ptr += dataSize;
	}
	//メモリにマッピング
	_emitterBin->Update(ptr, binSize, offset);
	WriteEmitterHeader(header);
	delete[] ptr;
}

void K3D::EmitterBinary::DeleteEmitter(int emitterIdx)
{
}

void K3D::EmitterBinary::Discard()
{

	this->_emitterBin.reset();
	this->_emitterHeadersBin.reset();
	this->_emitterTableBin.reset();

}

HRESULT K3D::EmitterBinary::EmitterBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = binarySize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _emitterBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE |
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);
	//破棄するまで永続マッピング
	_emitterBin->Map(0, nullptr);

	return hr;
}

HRESULT K3D::EmitterBinary::EmitterHeaderesBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = binarySize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _emitterBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE |
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);
	//破棄するまで永続マッピング
	_emitterHeadersBin->Map(0, nullptr);

	return hr;
}

HRESULT K3D::EmitterBinary::EmitterTableBinInit(std::shared_ptr<D3D12Device>& device, unsigned int binarySize)
{
	D3D12_HEAP_PROPERTIES prop = {};
	D3D12_RESOURCE_DESC   desc = {};
	prop.Type = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = binarySize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS |
		D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	auto hr = _emitterBin->Initialize(device, prop, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, desc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE |
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	CHECK_RESULT(hr);
	//破棄するまで永続マッピング
	_emitterTableBin->Map(0, nullptr);

	return hr;;
}

void K3D::EmitterBinary::WriteEmitterHeader(const CPUEmitterHeader * header,unsigned int index)
{
	unsigned offset = header->EmitterBinHead;
	unsigned binSize = sizeof(CPUEmitterHeader);

	_emitterHeadersBin->Update(header, binSize, index*binSize);

}
