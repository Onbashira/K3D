#include <string>
#include "CommandSignature.h"
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Signature/RootSignature.h"

K3D::CommandSignature::CommandSignature()  :
	_name("UnNamedCommandSignature")
{
}


K3D::CommandSignature::~CommandSignature()
{
	Discard();
}


HRESULT K3D::CommandSignature::Create(const D3D12_COMMAND_SIGNATURE_DESC & desc, std::weak_ptr<K3D::RootSignature> rs)
{
	auto res = Framework::GetDevice().GetDevice()->CreateCommandSignature(&desc, rs.lock()->GetSignature().Get(), IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

HRESULT K3D::CommandSignature::Create(const D3D12_COMMAND_SIGNATURE_DESC & desc)
{
	auto res = Framework::GetDevice().GetDevice()->CreateCommandSignature(&desc, nullptr, IID_PPV_ARGS(&_commandSignature));
	return E_NOTIMPL;
}

Microsoft::WRL::ComPtr<ID3D12CommandSignature> K3D::CommandSignature::GetSignature()
{
	return this->_commandSignature;
}

void K3D::CommandSignature::SetName(std::string name)
{
	if (_commandSignature.Get() != nullptr) {
		this->_commandSignature->SetName(Util::StringToWString(name).c_str());
		_name = name;
	}
}

void K3D::CommandSignature::Discard()
{
	if (_commandSignature.Get() != nullptr) {
		_commandSignature.Reset();
		DEBUG_LOG(std::string("CommandSignature : " + _name + " is  Reset"));
	}
}


