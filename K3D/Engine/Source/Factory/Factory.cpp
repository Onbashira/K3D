#include "Factory.h"
#include "Engine/Source/Utility/Logger.h"
#include <string>


K3D::Factory::Factory() :
	_factory()
{
}


K3D::Factory::~Factory()
{
	_factory.Reset();
}


HRESULT K3D::Factory::Create(UINT createFlags)
{
#if defined(_DEBUG)
	// DirectX12�̃f�o�b�O���C���[��L���ɂ���
	{
		Microsoft::WRL::ComPtr<ID3D12Debug>	debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

		}
	}
	createFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DirectX12���T�|�[�g���闘�p�\�ȃn�[�h�E�F�A�A�_�v�^���擾
	auto hr = CreateDXGIFactory2(createFlags, IID_PPV_ARGS(&_factory));
	if (FAILED(hr))
		return FALSE;
	return S_OK;
}

Microsoft::WRL::ComPtr<IDXGIFactory5> K3D::Factory::GetFactory()const
{
	return _factory;
}

void K3D::Factory::Discard()
{
	if (_factory != nullptr) {
		_factory.Reset();
		DEBUG_LOG(std::string("Factory is Reset"));
	}
}

