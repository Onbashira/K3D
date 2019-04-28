#pragma once
#include "Engine/ThirdParty/D3D12RaytracingFallback/Include/D3D12RaytracingFallback.h"

namespace K3D {

	class SwapChain;
	class D3D12Device;
	

	class DXRDevice
	{
	public:
		enum class RaytracingAPI {
			FallbackLayer,
			DirectXRaytracing,
		};
	private:

		bool _isFallbackSupported;

		bool _isDXRSupported;

		RaytracingAPI _raytracingAPI;

		//fallback attribute 

		Microsoft::WRL::ComPtr< ID3D12RaytracingFallbackDevice> _fallbackDevice;

		Microsoft::WRL::ComPtr< ID3D12RaytracingFallbackCommandList > _fallbackCommandList;

		Microsoft::WRL::ComPtr< ID3D12RaytracingFallbackStateObject > _fallbackStateObject;

	public:

		DXRDevice();

		~DXRDevice();

		HRESULT Initialize(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter);
	
	private:
		
	};

}