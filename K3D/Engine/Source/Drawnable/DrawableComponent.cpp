#include "DrawableComponent.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"

K3D::DrawableComponent::DrawableComponent()  :
	_commandList()
{
	_bundleList.Create(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE);
}


K3D::DrawableComponent::~DrawableComponent()
{
	this->_commandList.reset();
	this->_rootSignature.reset();
	this->_pipelineState.reset();
	this->_bundleList.Discard();
}


void K3D::DrawableComponent::BindingShaderObject()
{
	_commandList.lock()->GetCommandList()->SetPipelineState(_pipelineState.lock()->GetPSO().Get());
	_commandList.lock()->GetCommandList()->SetGraphicsRootSignature(_rootSignature.lock()->GetSignature().Get());
}

void K3D::DrawableComponent::BindingShaderObjectToBundle()
{
	_bundleList.GetCommandList()->SetPipelineState(_pipelineState.lock()->GetPSO().Get());
	_bundleList.GetCommandList()->SetGraphicsRootSignature(_rootSignature.lock()->GetSignature().Get());
}

void K3D::DrawableComponent::SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState)
{
	_pipelineState = pipelineState;
}

void K3D::DrawableComponent::SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature)
{
	_rootSignature = rootSignature;
}

void K3D::DrawableComponent::SetCommandList(std::shared_ptr<CommandList> masterCommandList)
{
	_commandList = masterCommandList;
}
