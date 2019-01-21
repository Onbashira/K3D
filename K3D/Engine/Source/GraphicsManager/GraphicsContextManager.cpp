#include "GraphicsContextManager.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Async/Fence.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PIpelineState/PipelineStateObject.h"

#include <memory>


K3D::GraphicsContextManager::GraphicsContextManager() :
	_commandListLibrary(), _shaderObjectLibrary(),_queueManager()
{

}


K3D::GraphicsContextManager::~GraphicsContextManager()
{
	Discard();
}


void K3D::GraphicsContextManager::CloseAllCommandLists()
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->CloseCommandList();
	}
}

void K3D::GraphicsContextManager::CloseCommandList(std::string commandListName)
{
	_commandListLibrary.Get(commandListName)->CloseCommandList();
}

void K3D::GraphicsContextManager::ResetAllCommandList()
{
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		itr->second->ResetAllocator();
		itr->second->ResetCommandList();
	}
}

void K3D::GraphicsContextManager::ResetCommandList(std::string commandListName)
{
	_commandListLibrary.Get(commandListName)->ResetAllocator();
	_commandListLibrary.Get(commandListName)->ResetCommandList();

}

HRESULT K3D::GraphicsContextManager::ExecutionCommandListAsync(CommandQueue * queue, std::vector<std::string> commandListNames, Fence * fence)
{

	std::vector<ID3D12CommandList*> lists(commandListNames.size());

	for (int i = 0; i < commandListNames.size(); ++i) {
		lists[i] = GetCommandList(commandListNames[i])->GetCommandList().Get();
	}

	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);

	return S_OK;
}

HRESULT K3D::GraphicsContextManager::ExecutionAllCommandLists(CommandQueue * queue, Fence* fence)
{

	std::vector< ID3D12CommandList *> lists;

	lists.reserve(_commandListLibrary._library.size());
	for (auto itr = _commandListLibrary._library.begin(); itr != _commandListLibrary._library.end(); itr++) {
		lists.push_back(itr->second->GetCommandList().Get());
	}
	queue->GetQueue()->ExecuteCommandLists(static_cast<unsigned int>(lists.size()), &lists[0]);
	if (fence != nullptr) {
		fence->Wait(queue);
	}
	return S_OK;
}

HRESULT K3D::GraphicsContextManager::CreateCommandList(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	return _commandListLibrary.Create(commandListName, nodeMask, listType);
}

HRESULT K3D::GraphicsContextManager::CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, gps, rootSignature);
}

HRESULT K3D::GraphicsContextManager::CreatePSO(std::shared_ptr<D3D12Device> device, std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(device,psoName, gps, rootSignature);
}

HRESULT K3D::GraphicsContextManager::CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC& cps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(psoName, cps, rootSignature);
}

HRESULT K3D::GraphicsContextManager::CreatePSO(std::shared_ptr<D3D12Device> device, std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC & cps, ID3DBlob * rootSignature)
{
	return _shaderObjectLibrary.CreatePSO(device,psoName, cps, rootSignature);
}

HRESULT K3D::GraphicsContextManager::CreateCommandQueue(std::string queueName, D3D12_COMMAND_QUEUE_DESC& desc)
{
	return this->_queueManager.Create(queueName,desc);
}

HRESULT K3D::GraphicsContextManager::CreateRootSignature(std::string rsName, ID3DBlob * signature)
{
	return _shaderObjectLibrary.CreateRootSignature(rsName, signature);
}

HRESULT K3D::GraphicsContextManager::CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC * signature)
{
	return _shaderObjectLibrary.CreateRootSignature(rsName, signature);
}

void K3D::GraphicsContextManager::SetCommandList(std::string commandListName, std::shared_ptr<CommandList> commandList)
{
	_commandListLibrary.Set(commandListName, commandList);
}

void K3D::GraphicsContextManager::SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso)
{
	_shaderObjectLibrary.SetPSO(psoName, pso);
}

void K3D::GraphicsContextManager::SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature)
{
	_shaderObjectLibrary.SetRootSignature(rsName, rootSignature);
}

std::shared_ptr<K3D::CommandList> K3D::GraphicsContextManager::GetCommandList(std::string commandListName)
{
	return _commandListLibrary.Get(commandListName);
}

std::shared_ptr<K3D::PipelineStateObject> K3D::GraphicsContextManager::GetPSO(std::string psoName)
{
	return _shaderObjectLibrary.GetPSO(psoName).lock();
}

std::shared_ptr<K3D::CommandQueue> K3D::GraphicsContextManager::GetCommandQueue(std::string queueName)
{
	return std::shared_ptr<CommandQueue>();
}

std::shared_ptr<K3D::RootSignature> K3D::GraphicsContextManager::GetRootSignature(std::string rsName)
{
	return _shaderObjectLibrary.GetRootSignature(rsName).lock();
}

void K3D::GraphicsContextManager::EraseCommandList(std::string commandListName)
{
	_commandListLibrary.Erase(commandListName);
}

void K3D::GraphicsContextManager::ErasePSO(std::string psoName)
{
	_shaderObjectLibrary.ErasePSO(psoName);
}

void K3D::GraphicsContextManager::EraseRootSignature(std::string rsName)
{
	_shaderObjectLibrary.EraseRootSignature(rsName);
}

void K3D::GraphicsContextManager::EraseCommandQueue(std::string queueName)
{
	_queueManager.Erase(queueName);
}

void K3D::GraphicsContextManager::Discard()
{
	_shaderObjectLibrary.Discard();
	_commandListLibrary.Discard();
}

K3D::GraphicsContextManager & K3D::GraphicsContextManager::GetInstance()
{
	static K3D::GraphicsContextManager instance;
	return instance;
}
