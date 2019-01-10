#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace K3D {

	class Factory;

	class D3D12Device
	{

	public:

	private:

		Microsoft::WRL::ComPtr<ID3D12Device3>	_device;

		D3D_FEATURE_LEVEL						_featureLevel;

		bool									_useWarpDevice;

	public:	

		D3D12Device();

		~D3D12Device();

		HRESULT									Create(Factory* factory, bool useWarpDevice = false);

		Microsoft::WRL::ComPtr<ID3D12Device3>	GetDevice()const;

		const D3D_FEATURE_LEVEL&				GetFeatureLevel()const;

		void									Discard();

	private:

	};

};