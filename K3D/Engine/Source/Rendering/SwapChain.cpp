#include "SwapChain.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Window/Window.h"


constexpr float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

K3D::SwapChain::SwapChain()
{
}


K3D::SwapChain::~SwapChain()
{
	Discard();
}


HRESULT K3D::SwapChain::CreateSwapChain(CommandQueue & commandQueue, std::shared_ptr<D3D12Device>& device, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{

	_width = windowWidth;
	_height = windowHeight;

	DXGI_SWAP_CHAIN_DESC1	swapChainDesc = {};
	swapChainDesc.BufferCount = bufferNum;
	swapChainDesc.Width = windowWidth;
	swapChainDesc.Height = windowHeight;
	swapChainDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	Microsoft::WRL::ComPtr<IDXGISwapChain1>	swapChain;
	if (FAILED(factory.GetFactory()->CreateSwapChainForHwnd(commandQueue.GetQueue().Get(), window.GetWindowHandle(), &swapChainDesc, nullptr, nullptr, &swapChain)))
		return FALSE;
	// フルスクリーンのサポートなし
	if (FAILED(factory.GetFactory()->MakeWindowAssociation(window.GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER)))
		return FALSE;

	if (FAILED(swapChain.As(&_swapChain)))
		return FALSE;

	_currentIndex = _swapChain->GetCurrentBackBufferIndex();
	return S_OK;
}

HRESULT K3D::SwapChain::CreateRenderTargets(std::shared_ptr<D3D12Device>& device,unsigned int bufferNum)
{
	this->_bufferNum = bufferNum;
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = bufferNum;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	_rtResource.resize(bufferNum);
	//レンダーターゲットビュー用のヒープの作成
	{
		if (FAILED(_rtHeap.Initialize(&desc)))
			return E_FAIL;
		_rtHeap.SetName("RendertargetsHeap ");
	}
	//レンダーターゲットの作成
	{
		for (UINT i = 0; i < bufferNum; i++) {
			_rtResource[i] = Resource::CreateShared();
			//ディスプレイバッファの取得
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(this->_rtResource[i]->GetResource().GetAddressOf()))))
			{
				Util::Comment(L"バックバッファの作成に失敗しました　エラーログを見てね");

				return FALSE;
			}
			//レンダーターゲットビューの作成
			device->GetDevice()->CreateRenderTargetView(_rtResource[i]->GetResource().Get(), nullptr, _rtHeap.GetCPUHandle(i));
			_rtResource[i]->SetResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
			_rtResource[i]->SetName(std::string("RenderTargetResource " + i));
		}
	}
	return S_OK;
}

HRESULT K3D::SwapChain::Initialize(CommandQueue & commandQueue, std::shared_ptr<D3D12Device>& device, Factory & factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum)
{
	auto hr = CreateSwapChain(commandQueue, device, factory, window, windowWidth, windowHeight, bufferNum);
	CHECK_RESULT(hr);
	hr = CreateRenderTargets(device,bufferNum);
	CHECK_RESULT(hr);
	return hr;
}

unsigned int K3D::SwapChain::GetBufferNum()
{
	return _bufferNum;
}

unsigned int K3D::SwapChain::GetCurrentBuckBuffer()
{
	return _currentIndex;
}

HRESULT K3D::SwapChain::SetStatePresent(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D::SwapChain::SetStateRenderTarget(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D::SwapChain::SetStateCopyDest(std::shared_ptr<CommandList> list)
{
	auto hr =_rtResource[_currentIndex]->ResourceTransition(list,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D::SwapChain::SetStateGenericRead(std::shared_ptr<CommandList> list)
{
	auto hr = _rtResource[_currentIndex]->ResourceTransition(list, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ);
	CHECK_RESULT(hr);
	return hr;
}

HRESULT K3D::SwapChain::CopyToRenderTarget(std::shared_ptr<CommandList> list,Resource* pSrc)
{
	list->GetCommandList()->CopyResource(this->_rtResource[_currentIndex]->GetResource().Get(),pSrc->GetResource().Get());
	return S_OK;
}

void K3D::SwapChain::SetRenderTarget(std::shared_ptr<CommandList> list , D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle)
{
	list->GetCommandList()->OMSetRenderTargets(1, &this->_rtHeap.GetCPUHandle(_currentIndex),FALSE, depthHandle);

}

void K3D::SwapChain::ClearScreen(std::shared_ptr<CommandList> list)
{
	//リソースステートをRTにバリアを張る
	SetStateRenderTarget(list);
	list->GetCommandList()->ClearRenderTargetView(this->_rtHeap.GetCPUHandle(_currentIndex), clearColor, 0, nullptr);

}

void K3D::SwapChain::ReSizeRenderTarget(std::shared_ptr<D3D12Device>& device, unsigned int width, unsigned int height, unsigned int backBufferNum)
{

	for (auto& ref : _rtResource) {
		ref->Discard();
	}

	if (width > 0 && height > 0) {
		_width = width;
		_height = height;
		_bufferNum = backBufferNum;
	}
	else
	{
		Util::Comment(L"スクリーンバッファのリサイズに失敗　サイズは0以上である必要があります");
		assert(0);
	}

	_swapChain->ResizeBuffers(
		_bufferNum,
		_width,
		_height,
		DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	//レンダーターゲットの作成
	{
		for (UINT i = 0; i < _bufferNum; i++) {
			//_rtResource[i] = Resource::CreateShared();
			//ディスプレイバッファの取得
			if (FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(this->_rtResource[i]->GetResource().GetAddressOf()))))
			{
				Util::Comment(L"バックバッファの作成に失敗しました　エラーログを見てね");
				assert(0);

			}
			//レンダーターゲットビューの作成
			device->GetDevice()->CreateRenderTargetView(_rtResource[i]->GetResource().Get(), nullptr, _rtHeap.GetCPUHandle(i));
			_rtResource[i]->SetResourceState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);
			_rtResource[i]->SetName(std::string("RenderTargetResource " + i));
		}
	}

}

void K3D::SwapChain::FlipScreen()
{
	_currentIndex = _swapChain->GetCurrentBackBufferIndex();
}

HRESULT K3D::SwapChain::Present(unsigned int sysncInterval, unsigned int flags)
{
	auto hr = _swapChain->Present(sysncInterval, flags);
	CHECK_RESULT(hr);
	FlipScreen();
	return hr;
}

void K3D::SwapChain::Discard()
{
	_swapChain.Reset();
	for (auto& res : _rtResource) {
		res->Discard();
	}
	_rtHeap.Discard();
}