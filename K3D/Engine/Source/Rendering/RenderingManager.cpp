#include "RenderingManager.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/Window/Window.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"

K3D::RenderingManager::~RenderingManager()
{
	Term();
}

HRESULT K3D::RenderingManager::Initialize(std::shared_ptr<D3D12Device>& device, Factory* factory, Window* window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = device->GetDevice()->GetNodeCount();
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	desc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	_masterQueue = std::make_shared<CommandQueue>();
	CHECK_RESULT(this->_masterQueue->Initialize(device,desc));
	CHECK_RESULT(_swapChain.Initialize(*this->_masterQueue.get(), *factory, *window, windowWidth, windowHeight, bufferNum));

	_renderingDevice = std::make_shared<RenderingDevice>();

	CHECK_RESULT(_renderingDevice->Initialize(device,_masterQueue,factory));

	return S_OK;
}

void K3D::RenderingManager::ClearScreen(std::weak_ptr<CommandList> list)
{
	_swapChain.ClearScreen(list.lock());
}

void K3D::RenderingManager::FlipScreen()
{
	_swapChain.FlipScreen();
}

void K3D::RenderingManager::CopyToRenderTarget(std::weak_ptr<CommandList> list, Resource* src)
{
	src->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	this->_swapChain.CopyToRenderTarget(list.lock(), src);
	src->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);

}

std::vector<std::shared_ptr<K3D::Resource>> K3D::RenderingManager::GetDisplayRenderTargets()
{
	return _swapChain._rtResource;
}

std::shared_ptr<K3D::CommandQueue> K3D::RenderingManager::GetQueue()
{
	return _masterQueue;
}

std::shared_ptr<K3D::RenderingDevice> K3D::RenderingManager::GetRenderingDevice()
{
	return _renderingDevice;
}

std::shared_ptr<K3D::RenderContext> K3D::RenderingManager::GetRenderContext()
{
	return _renderContext;
}

HRESULT K3D::RenderingManager::CreateCommandList(D3D12_COMMAND_LIST_TYPE & type, std::shared_ptr<CommandList>& commandList)
{

	return _renderContext->CreateCommandList(_renderingDevice->GetD3D12Device(), type, commandList);
}

void K3D::RenderingManager::Term()
{
	_swapChain.Discard();
	_masterQueue->Discard();
}
