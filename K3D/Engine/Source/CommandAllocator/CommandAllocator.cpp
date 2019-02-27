#include "stdafx.h"
#include "CommandAllocator.h"
#include "Engine/Source/CoreSystem/Framework.h"

K3D::CommandAllocator::CommandAllocator()
{
}


K3D::CommandAllocator::~CommandAllocator()
{
	Discard();
}

HRESULT K3D::CommandAllocator::Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	
	auto result = K3D::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandAllocator(listType, IID_PPV_ARGS(&_allocator));
	if (result != S_OK) {
		return E_FAIL;
	}
	return result;
}

HRESULT K3D::CommandAllocator::Create(D3D12Device * device, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	auto result = device->GetDevice()->CreateCommandAllocator(listType, IID_PPV_ARGS(&_allocator));
	if (result != S_OK) {
		return E_FAIL;
	}
	return result;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& K3D::CommandAllocator::GetAllocator()
{
	return this->_allocator;
}

void K3D::CommandAllocator::ResetAllocator()
{
	this->_allocator->Reset();
}

void K3D::CommandAllocator::Discard()
{
	if (this->_allocator != nullptr)
	{
		this->_allocator.Reset();
	}
}