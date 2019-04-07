#include "Fence.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CoreSystem/Framework.h"

K3D::Fence::Fence() :_fence(),_fenceValue(), _fenceEvent(nullptr)
{
}


K3D::Fence::~Fence()
{
	Discard();
}


HRESULT K3D::Fence::Initialize(UINT64 initialFenceValue, D3D12_FENCE_FLAGS flags)
{
	auto hresult = K3D::Framework::GetInstance().GetDevice()->GetDevice()->CreateFence(initialFenceValue,flags,IID_PPV_ARGS(&_fence));
	if (FAILED(hresult)) {
		return hresult;
	}
	_fenceValue = initialFenceValue;
	//_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	//if (_fenceEvent == nullptr) {
	//	return ERROR_INVALID_HANDLE;
	//}
	return hresult;
}

void K3D::Fence::Discard()
{
	CloseHandle(_fenceEvent);
	_fenceEvent = nullptr;
	_fenceValue = 0;
	_fence.Reset();
}

Microsoft::WRL::ComPtr<ID3D12Fence> K3D::Fence::GetFence()
{
	return _fence;
}

UINT64 K3D::Fence::GetFenceValue()
{
	return _fenceValue;
}

void * K3D::Fence::GetFenceEvent()
{
	return _fenceEvent;
}

HRESULT K3D::Fence::Wait(CommandQueue * commandQueue)
{
	const UINT64 fence = _fenceValue;
	auto hr = commandQueue->GetQueue()->Signal(_fence.Get(), fence);
	if (FAILED(hr))
		return hr;
	_fenceValue++;

	// Š®—¹‚ð‘Ò‹@.
	if (_fence->GetCompletedValue() < fence)
	{
		hr = _fence->SetEventOnCompletion(fence, _fenceEvent);
		if (FAILED(hr))
			return hr;
		WaitForSingleObject(_fenceEvent, INFINITE);
	}
	return S_OK;
}
