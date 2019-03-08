#include "RenderingDevice.h"
#include "Engine/Source/Factory/Factory.h"

//対応フィーチャーレベル
const D3D_FEATURE_LEVEL FeatureLevels[] =
{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0
};
const std::wstring GPU_CARD_SPEC_LIST[] = {

	L"NVIDIA GeForce GTX 1080Ti",
	L"NVIDIA GeForce GTX 1080",
	L"NVIDIA GeForce GTX 1070Ti",
	L"NVIDIA GeForce GTX 1070",
	L"NVIDIA GeForce GTX 1060",
	L"NVIDIA GeForce GTX 1050Ti",
	L"NVIDIA GeForce GTX 1050",

	L"Intel HD Graphics"

};

K3D::RenderingDevice::RenderingDevice()
{
	
}


K3D::RenderingDevice::~RenderingDevice()
{
	Discard();
}

HRESULT K3D::RenderingDevice::Create(Factory * factory, int node, bool useWarpDevice)
{
	HRESULT result = {};
	_useWarpDevice = useWarpDevice;
	if (_useWarpDevice) {
		Microsoft::WRL::ComPtr<IDXGIAdapter>	warpAdapter;

		if (FAILED(factory->GetFactory()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)))) {
			return E_FAIL;
		}
		for (auto i : FeatureLevels) {
			if (SUCCEEDED(D3D12CreateDevice(warpAdapter.Get(), i, IID_PPV_ARGS(&_device)))) {
				_featureLevel = i;
				break;
			}
		}

	}
	else {
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	hardwareAdapter;
		Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
		hardwareAdapter = nullptr;

		for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->GetFactory()->EnumAdapters1(i, &adapter); i++) {
			//アダプタの取得
			adapter->GetDesc1(&_adapterDesc);
			if (_adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
				continue;
			}

			for (auto i : FeatureLevels) {
				//アダプタが想定したFeatureLevelに対応しているか、またそのレベルでデバイスが作成できるか

				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), i, _uuidof(ID3D12Device3), nullptr))) {
					_featureLevel = i;
					hardwareAdapter = adapter;
					for (auto& spec : GPU_CARD_SPEC_LIST) {
						if (spec == std::wstring(&_adapterDesc.Description[0])) {
							result = D3D12CreateDevice(hardwareAdapter.Get(), _featureLevel, IID_PPV_ARGS(&_device));
							if (FAILED(result)) {
								continue;
							}
						}
						return result;
					}
				}
			}
		}
		adapter.Reset();
	}
	return result;
}

Microsoft::WRL::ComPtr<ID3D12Device3> K3D::RenderingDevice::GetDevice() const
{
	return _device;
}

const D3D_FEATURE_LEVEL & K3D::RenderingDevice::GetFeatureLevel() const
{
	return _featureLevel;
}

const K3D::String & K3D::RenderingDevice::GetDeviceName() const
{
	return _deviceName;
}

void K3D::RenderingDevice::Discard()
{
	if (this->_device.Get() != nullptr) {
		_device.Reset();
	}
}
