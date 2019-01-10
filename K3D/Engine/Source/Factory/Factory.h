#pragma once
#include"Engine/Source/Utility/D3D12Common.h"


namespace K3D {
	
	class Factory
	{

	public:

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory5>	_factory;

	public:

		Factory();

		~Factory();

		HRESULT	Create(UINT createFlags = 0);

		Microsoft::WRL::ComPtr<IDXGIFactory5> GetFactory()const;

		void Discard();

	private:

	};

}