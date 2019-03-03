#pragma once
namespace K3D {
	class Factory;
	class RenderingDevice
	{
	public:

	private:

		int _deviceNode;

		D3D_FEATURE_LEVEL _featureLevel;

		DXGI_ADAPTER_DESC1 _adapterDesc;

		bool _useWarpDevice;

		Microsoft::WRL::ComPtr<ID3D12Device3> _device;

		String _deviceName;

		static inline std::vector<String> _usedGPUName;

	public:
		
		RenderingDevice();
		
		~RenderingDevice();

		HRESULT Create(Factory* factory,int node,bool useWarpDevice = false);

		Microsoft::WRL::ComPtr<ID3D12Device3> GetDevice()const;

		const D3D_FEATURE_LEVEL& GetFeatureLevel()const;

		const String& GetDeviceName()const;

		void Discard();


	};

}