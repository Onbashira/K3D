#include "stdafx.h"
#include "RenderContext.h"
#include "Engine/Source/CommandAllocator/CommandAllocator.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"

K3D::RenderContext::RenderContext()
{

}


K3D::RenderContext::~RenderContext()
{

}

HRESULT K3D::RenderContext::Create(int frameNum, int nodeMask, std::shared_ptr<CommandQueue>& queue)
{
	this->_currentIndex = 0;
	this->_node = nodeMask;
	this->_frameNum = frameNum;
	HRESULT hret = {};
	//CreateAllocator
	{
		this->_cmdAllocators.resize(_frameNum);
		for (int i = 0; i < frameNum;++i) {
			_cmdAllocators[i] = std::make_shared<CommandAllocator>();
			hret = _cmdAllocators[i]->Create(nodeMask, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
			if (FAILED(hret))
				return hret;
		}
	}
	//CreateCommandList
	{
		this->_cmdLists.resize(_frameNum);
		for (int i = 0; i < frameNum; ++i) {
			_cmdLists[i][0] = std::make_shared<CommandList>();
			_cmdLists[i][1] = std::make_shared<CommandList>();

			_cmdLists[i][0]->Create(nodeMask,
				D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
			if (FAILED(hret))
				return hret;
			_cmdLists[i][1]->Create(nodeMask, 
				D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
			if (FAILED(hret))
				return hret;
		}

	}
	//CreateFence
	{
		this->_fences.resize(_frameNum);
		for (int i = 0; i < frameNum; ++i) {

			_fences[i].Create(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE);
			if (FAILED(hret))
				return hret;
		}
	}
	return E_NOTIMPL;
}

int K3D::RenderContext::GetCurrentIndex()
{
	return 0;
}

int K3D::RenderContext::IncrementCount()
{
	return 0;
}

std::weak_ptr<K3D::CommandList> K3D::RenderContext::GetCurrentList(RenderCommandList & listType)
{
	return std::weak_ptr<K3D::CommandList>();
}

std::weak_ptr<K3D::CommandList> K3D::RenderContext::GetCurrentCmdAllocator()
{
	return std::weak_ptr<K3D::CommandList>();
}

K3D::Fence & K3D::RenderContext::GetCurrentFence()
{
	// TODO: return ステートメントをここに挿入します
}

void K3D::RenderContext::ResetAllocators()
{
}

void K3D::RenderContext::ResetCurrentCommandList()
{
}

void K3D::RenderContext::ResetCommandList(std::shared_ptr<CommandList>& list)
{
}

void K3D::RenderContext::Reset()
{
}

void K3D::RenderContext::Discard()
{
}
