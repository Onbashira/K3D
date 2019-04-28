#include "stdafx.h"
#include "DXRDevice.h"
#include "Engine/ThirdParty/D3D12RaytracingFallback/Include/D3D12RaytracingHelpers.hpp"

// Enable experimental features required for compute-based raytracing fallback.
// This will set active D3D12 devices to DEVICE_REMOVED state.
// Returns bool whether the call succeeded and the device supports the feature.
bool EnableComputeRaytracingFallback(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter);
// Returns bool whether the device supports DirectX Raytracing tier.
bool IsDirectXRaytracingSupported(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter);


K3D::DXRDevice::DXRDevice()
{
}


K3D::DXRDevice::~DXRDevice()
{
}

HRESULT K3D::DXRDevice::Initialize(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter)
{// Fallback Layer uses an experimental feature and needs to be enabled before creating a D3D12 device.
	_isFallbackSupported = EnableComputeRaytracingFallback(adapter);

	if (!_isFallbackSupported)
	{
		OutputDebugString(
			L"Warning: Could not enable Compute Raytracing Fallback (D3D12EnableExperimentalFeatures() failed).\n" \
			L"         Possible reasons: your OS is not in developer mode.\n\n");
	}

	_isDXRSupported = IsDirectXRaytracingSupported(adapter);

	if (!_isDXRSupported)
	{
		OutputDebugString(L"Warning: DirectX Raytracing is not supported by your GPU and driver.\n\n");

		//ThrowIfFalse(_isFallbackSupported,
		//	L"Could not enable compute based fallback raytracing support (D3D12EnableExperimentalFeatures() failed).\n"\
		//	L"Possible reasons: your OS is not in developer mode.\n\n");
		_raytracingAPI = RaytracingAPI::FallbackLayer;
	}


}

bool IsDirectXRaytracingSupported(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter)
{

	Microsoft::WRL::ComPtr<ID3D12Device> testDevice;
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupportData = {};

	return SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&testDevice)))
		&& SUCCEEDED(testDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupportData, sizeof(featureSupportData)))
		&& featureSupportData.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

}

bool EnableComputeRaytracingFallback(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter)
{

	Microsoft::WRL::ComPtr<ID3D12Device> testDevice;
	UUID experimentalFeatures[] = { D3D12ExperimentalShaderModels };

	return SUCCEEDED(D3D12EnableExperimentalFeatures(1, experimentalFeatures, nullptr, nullptr))
		&& SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&testDevice)));

}
