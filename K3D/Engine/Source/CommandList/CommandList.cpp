#include "CommandList.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/CoreSystem/Framework.h"

K3D::CommandList::CommandList() :
	_commandList(), _commandAllocator(), 
	_commandListName("UnNamed_CommandList"), 
	_commandAllocatorName("UnNamed_CommandAllocator")
{

}


K3D::CommandList::~CommandList()
{
	Discard();
}


HRESULT K3D::CommandList::Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	_listType = listType;
	HRESULT result;
	result = K3D::Framework::GetDevice().GetDevice()->CreateCommandAllocator(_listType, IID_PPV_ARGS(&_commandAllocator));
	if (result != S_OK) {
		return E_FAIL;
	}

	result = K3D::Framework::GetDevice().GetDevice()->CreateCommandList(nodeMask, _listType, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(&_commandList));
	if (result != S_OK) {
		return E_FAIL;
	}


	return S_OK;
}

HRESULT K3D::CommandList::SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	D3D12_RESOURCE_BARRIER resource_barrier{};

	resource_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	resource_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	resource_barrier.Transition.pResource = resource;
	resource_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	resource_barrier.Transition.StateBefore = beforeState;
	resource_barrier.Transition.StateAfter = afterState;

	_commandList->ResourceBarrier(1, &resource_barrier);
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> K3D::CommandList::GetCommandList()const
{
	return this->_commandList;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> K3D::CommandList::GetAllocator()const
{
	return this->_commandAllocator;
}

HRESULT K3D::CommandList::ResetCommandList(ID3D12PipelineState * pInitialState)
{
	auto hr = _commandList->Reset(_commandAllocator.Get(), pInitialState);
	return hr;
}

HRESULT K3D::CommandList::ResetAllocator()
{
	auto  hr = _commandAllocator->Reset();
	return hr;
}

HRESULT K3D::CommandList::CloseCommandList()
{
	auto hr = _commandList->Close();
	return hr;
}



void K3D::CommandList::Discard()
{

	if (_commandList.Get() != nullptr) {
		if (_commandListName == "UnNamed_CommandList") {
			assert(true);

		}
		this->_commandAllocator.Reset();
		this->_commandList.Reset();
		DEBUG_LOG(std::string("CommandList : " + _commandListName + " is Reset"));
		DEBUG_LOG(std::string("CommandAllocator : " + _commandAllocatorName + " is Reset"));

	}

}

void K3D::CommandList::SetName(std::string objectName)
{
	_commandListName = objectName + "List";
	SetCommandListName(_commandListName);
	_commandAllocatorName = objectName + "Allocator";
	SetCommandAllocatorName(_commandAllocatorName);
}

void K3D::CommandList::SetCommandListName(std::string name)
{
	_commandListName = name;
	this->_commandList->SetName(Util::StringToWString(_commandListName).c_str());
}

void K3D::CommandList::SetCommandAllocatorName(std::string name)
{
	_commandAllocatorName = name;
	this->_commandAllocator->SetName(Util::StringToWString(_commandAllocatorName).c_str());
}