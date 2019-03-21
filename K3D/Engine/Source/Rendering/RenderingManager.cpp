#include "RenderingManager.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/Window/Window.h"

K3D::RenderingManager::~RenderingManager()
{
	Term();
}

HRESULT K3D::RenderingManager::Initialize(std::shared_ptr<D3D12Device> device, Factory* factory, Window* window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{
	CHECK_RESULT(_swapChain.Initialize(*this->_masterQueue.get(), *factory, *window, windowWidth, windowHeight, bufferNum));
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

std::vector<std::shared_ptr<K3D::Resource>> K3D::RenderingManager::GetMasterRenderTargets()
{
	return _swapChain._rtResource;
}

void K3D::RenderingManager::Term()
{
	_swapChain.Discard();
}
