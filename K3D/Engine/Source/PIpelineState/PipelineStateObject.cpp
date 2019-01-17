#include "PipelineStateObject.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/Debug/Logger.h"
#include "Engine/Source/Utility/Utility.h"
#include <string>

K3D::PipelineStateObject::PipelineStateObject() :
	_pipelineState(), _name("UnNamed PipelineState")
{

}

K3D::PipelineStateObject::~PipelineStateObject()
{
	Discard();
}


HRESULT K3D::PipelineStateObject::Create(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}
	auto hr = Framework::GetDevice().GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	return hr;
}

HRESULT K3D::PipelineStateObject::Create(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, ID3DBlob * shader)
{
	if (shader != nullptr) {
		GraphicsContextManager::GetInstance().CreateRootSignature(name + "RootSignature", shader);
		_rootSignature = GraphicsContextManager::GetInstance().GetRootSignature(name + "RootSignature");
		cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	}

	auto hr = Framework::GetDevice().GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT K3D::PipelineStateObject::Create(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	gpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();
	auto hr = Framework::GetDevice().GetDevice()->CreateGraphicsPipelineState(&gpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

HRESULT K3D::PipelineStateObject::Create(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC & cpsDesc, std::weak_ptr<RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
	cpsDesc.pRootSignature = _rootSignature.lock()->GetSignature().Get();

	auto hr = Framework::GetDevice().GetDevice()->CreateComputePipelineState(&cpsDesc, IID_PPV_ARGS(&_pipelineState));
	SetName(name);

	return hr;
}

void K3D::PipelineStateObject::SetName(std::string name)
{
	_pipelineState->SetName(Util::StringToWString(name).c_str());
	_name = name;
}

void K3D::PipelineStateObject::BindingRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature)
{

}

std::weak_ptr<K3D::RootSignature> K3D::PipelineStateObject::GetRootSignature()
{
	return this->_rootSignature;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> K3D::PipelineStateObject::GetPSO()
{
	return this->_pipelineState;
}

void K3D::PipelineStateObject::Discard()
{
	if (_pipelineState.Get() != nullptr) {
		_pipelineState.Reset();
		
		DEBUG_LOG(std::string("PipelineState : " + _name + " is  Reset"));
	}
	_rootSignature.reset();
}
