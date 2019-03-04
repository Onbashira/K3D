#include "CommandQueue.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"

K3D::CommandQueue::CommandQueue() : _commandQueue()
{
}


K3D::CommandQueue::~CommandQueue()
{
	Discard();
}

HRESULT K3D::CommandQueue::Create(D3D12_COMMAND_QUEUE_DESC& desc)
{
	CHECK_RESULT(K3D::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue)));
	CHECK_RESULT(_fence.Create(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE));

	return S_OK;
}

HRESULT K3D::CommandQueue::Create(std::weak_ptr<D3D12Device> device, D3D12_COMMAND_QUEUE_DESC & desc)
{
	CHECK_RESULT(K3D::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue)));
	CHECK_RESULT(_fence.Create(0, D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE));

	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& K3D::CommandQueue::GetQueue()
{
	return _commandQueue;
}

void K3D::CommandQueue::Wait(Fence * fence)
{
	if (fence != nullptr) {
		fence->Wait(this);
	}
	else {
		_fence.Wait(this);
	}
}

UINT64 K3D::CommandQueue::GetTimestampFrequency()
{
	if (_commandQueue == nullptr) {
		return 1;
	}

	UINT64 ret;
	auto hr = _commandQueue->GetTimestampFrequency(&ret);
	if (FAILED(hr)) {
		return 1;
	}

	return ret;
}

D3D12_COMMAND_QUEUE_DESC& K3D::CommandQueue::GetDesc()
{
	return _commandQueue->GetDesc();
}

void K3D::CommandQueue::ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists)
{
	std::vector < ID3D12CommandList* > rawLists(lists.size());
	auto ptr = rawLists.data();
	for (auto& list : lists) {
		(*ptr = list->GetCommandList().Get())++;
	}
	this->_commandQueue->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), rawLists.data());
}

void K3D::CommandQueue::SetName(std::string name)
{
	this->_commandQueue->SetName(Util::StringToWString(name).c_str());
}

void K3D::CommandQueue::Discard()
{
	if (_commandQueue.Get() != nullptr) {
		_commandQueue.Reset();
		DEBUG_LOG(std::string(_name + " is Reset"));
	}
}


