#include "CommandQueue.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"

K3D::CommandQueue::CommandQueue() : _3DQueue()
{
}


K3D::CommandQueue::~CommandQueue()
{
	Discard();
}

HRESULT K3D::CommandQueue::Initialize(D3D12_COMMAND_QUEUE_DESC& desc)
{
	CHECK_RESULT(K3D::Framework::GetInstance().GetDevice()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_3DQueue)));

	return S_OK;
}

HRESULT K3D::CommandQueue::Initialize(std::weak_ptr<D3D12Device> device, D3D12_COMMAND_QUEUE_DESC & desc)
{
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_3DQueue)));
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_copyQueue)));
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
	CHECK_RESULT(device.lock()->GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_computeQueue)));
	return S_OK;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& K3D::CommandQueue::GetQueue()
{
	return _3DQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& K3D::CommandQueue::GetCopyQueue()
{
	return _copyQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& K3D::CommandQueue::GetComputeQueue()
{
	return _computeQueue;
}


UINT64 K3D::CommandQueue::GetTimestampFrequency()
{
	if (_3DQueue == nullptr) {
		return 1;
	}

	UINT64 ret;
	auto hr = _3DQueue->GetTimestampFrequency(&ret);
	if (FAILED(hr)) {
		return 1;
	}

	return ret;
}

D3D12_COMMAND_QUEUE_DESC& K3D::CommandQueue::GetDesc()
{
	return _3DQueue->GetDesc();
}

void K3D::CommandQueue::ExecuteCommandLists(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _3DQueue);
	DETAILS_LOG(String("Execute 3DQueue"));
}

void K3D::CommandQueue::ExecuteCopyCommands(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _copyQueue);
	DETAILS_LOG(String("Execute CopyQueue"));

}

void K3D::CommandQueue::ExecuteComputeCommands(std::vector<std::shared_ptr<CommandList>>& lists)
{
	Execute(lists, _computeQueue);
	DETAILS_LOG(String("Execute ComputeQueue"));

}

void K3D::CommandQueue::SetName(std::string name)
{
	this->_3DQueue->SetName(Util::StringToWString(name + "_3DQueue").c_str());
	this->_copyQueue->SetName(Util::StringToWString(name + "_CopyQueue").c_str());
	this->_computeQueue->SetName(Util::StringToWString(name + "_ComputeQueue").c_str());

}

void K3D::CommandQueue::Discard()
{
	if (_3DQueue.Get() != nullptr) {
		_3DQueue.Reset();
		_copyQueue.Reset();
		_computeQueue.Reset();

		DEBUG_LOG(std::string(_name + " is Reset"));
	}
}

void K3D::CommandQueue::Execute(std::vector<std::shared_ptr<CommandList>>& lists, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& queue)
{
	std::vector < ID3D12CommandList* > rawLists(lists.size());
	auto ptr = rawLists.data();
	for (auto& list : lists) {
		*ptr = list->GetCommandList().Get();
		ptr++;
	}
	queue->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), rawLists.data());
}