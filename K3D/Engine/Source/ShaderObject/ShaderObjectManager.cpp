#include "ShaderObjectManager.h"
#include "Engine/Source/Utility/Logger.h"
#include "Engine/Source/Signature/CommandSignature.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PIpelineState/PipelineStateObject.h"




K3D::ShaderObjectManager::ShaderObjectManager()
{
}


K3D::ShaderObjectManager::~ShaderObjectManager()
{
	Discard();
}


HRESULT K3D::ShaderObjectManager::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		HRESULT hr = {};
		this->_psolibrary[psoName] = std::make_shared<K3D::PipelineStateObject>();
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName, signature);
			hr = this->_psolibrary[psoName]->Create(psoName, gps, GetRootSignature(psoName ));

		}
		else {
			hr = this->_psolibrary[psoName]->Create(psoName, gps, nullptr);
		}
		if (SUCCEEDED(hr)) {

			DEBUG_LOG(std::string("PSO������ɍ쐬�E�o�^����܂��� : " + psoName ));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D::ShaderObjectManager::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC cps, ID3DBlob * signature)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = std::make_shared<K3D::PipelineStateObject>();
		HRESULT hr = {};
		if (signature != nullptr) {
			hr = CreateRootSignature(psoName + "RootSignature", signature);
			hr = this->_psolibrary[psoName]->Create(psoName, cps, GetRootSignature(psoName + "RootSignature"));

		}
		else {
			hr = this->_psolibrary[psoName]->Create(psoName, cps, nullptr);
		}
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("PSO������ɍ쐬�E�o�^����܂��� : " + psoName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D::ShaderObjectManager::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<K3D::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromShader(signature);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature������ɍ쐬�E�o�^����܂��� : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

HRESULT K3D::ShaderObjectManager::CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC * desc)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = std::make_shared<K3D::RootSignature>();
		auto hr = this->_rootSignaturelibrary[rsName]->CreateFromDesc(desc);
		this->_rootSignaturelibrary[rsName]->SetName(rsName);

		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string("RootSignature������ɍ쐬�E�o�^����܂��� : " + rsName));

			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void K3D::ShaderObjectManager::SetPSO(std::string psoName, std::shared_ptr<K3D::PipelineStateObject> pso)
{
	if (_psolibrary.find(psoName) == _psolibrary.end()) {
		this->_psolibrary[psoName] = pso;
		DEBUG_LOG(std::string("PSO��o�^���܂��� : " + psoName));

	}
}

void K3D::ShaderObjectManager::SetRootSignature(std::string rsName, std::shared_ptr<K3D::RootSignature> rootSignature)
{
	if (_rootSignaturelibrary.find(rsName) == _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary[rsName] = rootSignature;
		DEBUG_LOG(std::string("RootSignature��o�^���܂��� : " + rsName ));
	}
}

std::weak_ptr<K3D::PipelineStateObject> K3D::ShaderObjectManager::GetPSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		return  this->_psolibrary[psoName];
	}
	ERROR_LOG(std::string("�s���������[�h�ł� : " + psoName ));

	return std::weak_ptr<K3D::PipelineStateObject>();
}

std::weak_ptr<K3D::RootSignature> K3D::ShaderObjectManager::GetRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		return  this->_rootSignaturelibrary[rsName];
	}
	ERROR_LOG(std::string("�s���������[�h�ł� : " + rsName));

	return std::weak_ptr<K3D::RootSignature>();
}

void K3D::ShaderObjectManager::ErasePSO(std::string psoName)
{
	if (_psolibrary.find(psoName) != _psolibrary.end()) {
		this->_psolibrary.erase(psoName);
		DEBUG_LOG(std::string("PSO���폜���܂��� : " + psoName));

	}
}

void K3D::ShaderObjectManager::EraseRootSignature(std::string rsName)
{
	if (_rootSignaturelibrary.find(rsName) != _rootSignaturelibrary.end()) {
		this->_rootSignaturelibrary.erase(rsName);
		DEBUG_LOG(std::string("RootSignature���폜���܂��� : " + rsName));

	}
}

void K3D::ShaderObjectManager::Discard()
{

	this->_psolibrary.clear();
	this->_rootSignaturelibrary.clear();
}
