#include "stdafx.h"
#include "RenderContext.h"
#include "Engine/Source/CommandAllocator/CommandAllocator.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"

K3D::RenderContext::RenderContext() : 
	_frameNum(0),_currentIndex(0),_node(0),_currentFence(0),
	_isDiscarded(false)
{

}


K3D::RenderContext::~RenderContext()
{
	Discard();
}

HRESULT K3D::RenderContext::Initialize(std::shared_ptr<D3D12Device>& device, int frameNum, int nodeMask, std::shared_ptr<CommandQueue>& queue)
{
	this->_currentIndex = 0;
	this->_currentFence = 0LL;
	this->_node = nodeMask;
	this->_frameNum = frameNum;
	HRESULT hret = {};

	this->_cmdAllocators.resize(_frameNum);
	this->_cmdLists.resize(_frameNum);
	this->_fences.resize(_frameNum);
	this->_listsVector.resize(_frameNum);
	for (int i = 0; i < frameNum; ++i) {
		_cmdAllocators[i] = std::make_shared<CommandAllocator>();

		hret = _cmdAllocators[i]->Initialize(device, nodeMask, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
		if (FAILED(hret))
			return hret;

		_cmdLists[i][0] = std::make_shared<CommandList>();
		_cmdLists[i][1] = std::make_shared<CommandList>();
		_cmdLists[i][0]->Initialize(device, nodeMask,
			D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocators[i]);
		if (FAILED(hret))
			return hret;
		_cmdLists[i][1]->Initialize(device, nodeMask,
			D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAllocators[i]);
		if (FAILED(hret))
			return hret;


		_fences[i].Initialize(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE);
		if (FAILED(hret))
			return hret;

	}
	return hret;

}

HRESULT K3D::RenderContext::CreateCommandList(std::shared_ptr<D3D12Device>& device, D3D12_COMMAND_LIST_TYPE & type, std::shared_ptr<CommandList>& commandList)
{
	return E_NOTIMPL;
}

int K3D::RenderContext::GetCurrentIndex()
{
	return _currentIndex;
}

int K3D::RenderContext::IncrementCount()
{
	_currentIndex = (_currentIndex + 1) % _frameNum;
	return _currentIndex;
}

std::weak_ptr<K3D::CommandList> K3D::RenderContext::GetResourceUpdateCmdList(RC_COMMAND_LIST_TYPE & listType)
{
	return _cmdLists[_currentIndex][static_cast<int>(listType)];
}

std::weak_ptr<K3D::CommandAllocator> K3D::RenderContext::GetCurrentCmdAllocator()
{
	return _cmdAllocators[_currentIndex];
}

K3D::Fence & K3D::RenderContext::GetCurrentFence()
{
	return _fences[_currentIndex];
}

std::weak_ptr<K3D::CommandQueue> K3D::RenderContext::GetCommandQueue()
{
	return _queueRef;
}

void K3D::RenderContext::ExecuteCommandLists(std::shared_ptr<CommandQueue>& commandQueue, bool executeNow = false)
{
	_currentFence++;
	commandQueue->GetQueue()->Signal(_fences[_currentIndex].GetFence().Get(), _currentFence);

	INT64 displayFence = _currentFence - _frameNum + 1;
	int displayIndex = _currentIndex;
	if (executeNow)
	{
		displayFence = _currentFence;
	}
	auto completeValue = _fences[_currentIndex].GetFence()->GetCompletedValue();
	if ((completeValue < displayFence && _currentFence >= _frameNum)
		|| executeNow && completeValue < _currentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

		_fences[_currentIndex].GetFence()->SetEventOnCompletion(displayFence, eventHandle);

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void K3D::RenderContext::ResetAllocators()
{
	std::lock_guard mutexLock(_allocatorMutex);

	for (auto& allocator : _cmdAllocators)
	{
		allocator->ResetAllocator();
	}
}

void K3D::RenderContext::ResetCurrentCommandAllocator()
{
	std::lock_guard mutexLock(_allocatorMutex);

	_cmdAllocators[_currentIndex]->ResetAllocator();
}

void K3D::RenderContext::ResetCommandList(std::shared_ptr<CommandList>& list)
{
	list->ResetCommandList(_cmdAllocators[_currentIndex]);
}

void K3D::RenderContext::Reset()
{
	_cmdAllocators[_currentIndex]->ResetAllocator();
	for (int i = 0; i < 2;++i) {
		_cmdLists[_currentIndex][i]->ResetCommandList(_cmdAllocators[_currentIndex],nullptr);
	}

}

void K3D::RenderContext::Discard()
{
	if (_isDiscarded) {
		return;
	}
	this->ResetAllocators();
	for (int i = 0; i < _frameNum; ++i) {
		this->_cmdAllocators[i]->Discard();
		this->_cmdAllocators[i].reset();
		this->_fences[i].Discard();
		for (int j = 2; j < 2; ++j) {
			this->_cmdLists[i][j]->Discard;
			this->_cmdLists[i][j].reset();
		}
		_listsVector[i].clear();

	}

	_isDiscarded = true;
}
