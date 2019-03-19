#pragma once


//https://docs.microsoft.com/en-us/windows/desktop/api/d3d11on12/nf-d3d11on12-d3d11on12createdevice

namespace K3D {

	class D3D12Device;
	class CommandQueue;

	class D3D11on12Device
	{
	public:
	private:

		Microsoft::WRL::ComPtr<ID3D11On12Device> _d3d11on12Device;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _d3d11DeviceContext;


	public:
		D3D11on12Device();
		virtual ~D3D11on12Device();

		HRESULT Init(std::shared_ptr<D3D12Device> d3d12Device, std::shared_ptr<CommandQueue> queue);
	
		Microsoft::WRL::ComPtr<ID3D11On12Device>& GetDevice();

	};
}