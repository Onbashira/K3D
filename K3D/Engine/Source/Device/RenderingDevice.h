#pragma once
namespace K3D {
	class Factory;
	class D3D12Device;
	class D3D11On12Device;
	class RenderingDevice
	{
	public:

	private:

	public:
		
		RenderingDevice();
		
		~RenderingDevice();

		HRESULT Create(Factory* factory,int node,bool useWarpDevice = false);

		Microsoft::WRL::ComPtr<ID3D12Device3> GetDevice()const;

		const D3D_FEATURE_LEVEL& GetFeatureLevel()const;

		const String& GetDeviceName()const;

		void Discard();

	private:

	};

}