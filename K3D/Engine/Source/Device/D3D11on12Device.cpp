#include "stdafx.h"
#include "D3D11on12Device.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"



K3D::D3D11on12Device::D3D11on12Device()
{
}


K3D::D3D11on12Device::~D3D11on12Device()
{
}

HRESULT K3D::D3D11on12Device::Init(std::shared_ptr<D3D12Device> d3d12Device, std::shared_ptr<CommandQueue> queue)
{
	HRESULT ret = S_OK;

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
	ID3D12CommandQueue* queues[] = { queue->GetQueue().Get() ,queue->GetCopyQueue,queue->GetComputeQueue().Get() };
	ret = D3D11On12CreateDevice(d3d12Device->GetDevice().Get, flags, nullptr, 0,
		reinterpret_cast<IUnknown**>(queues), _countof(queues), 0,&d3d11Device,
		&_d3d11DeviceContext,nullptr);
	if (FAILED(ret))
	{
		return ret;
	}

	ret = d3d11Device.As(&_d3d11on12Device);

	if (FAILED(ret)) {

		return ret;

	}
#ifdef _DEBUG

	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;


#endif // _DEBUG


 }

Microsoft::WRL::ComPtr<ID3D11On12Device>& K3D::D3D11on12Device::GetDevice()
{
	return this->_d3d11on12Device;
}
