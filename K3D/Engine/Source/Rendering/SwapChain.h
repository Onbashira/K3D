#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Resource/Resource.h"
#include <memory>
#include <vector>

namespace K3D {
	class CommandList;
	class CommandQueue;
	class Factory;
	class Window;

	class SwapChain
	{	
	
	public:
	
	private:
		unsigned int _bufferNum;

		unsigned int _currentIndex;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;

		std::vector<std::shared_ptr<Resource>> _rtResource;

		DescriptorHeap _rtHeap;

	public:

		SwapChain();

		~SwapChain();

		HRESULT Initialize(CommandQueue& commandQueue, Factory& factory, Window & window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		unsigned int GetBufferNum();

		unsigned int GetCurrentBuckBuffer();

		HRESULT SetStatePresent(std::shared_ptr<CommandList> list);

		HRESULT SetStateRenderTarget(std::shared_ptr<CommandList> list);

		HRESULT	SetStateCopyDest(std::shared_ptr<CommandList> list);

		HRESULT	SetStateGenericRead(std::shared_ptr<CommandList> list);

		HRESULT	CopyToRenderTarget(std::shared_ptr<CommandList> list, Resource* pSrc);

		void SetRenderTarget(std::shared_ptr<CommandList> list, D3D12_CPU_DESCRIPTOR_HANDLE* depthHandle = nullptr);

		void ClearScreen(std::shared_ptr<CommandList> list);

		void FlipScreen();

		HRESULT Present(unsigned int sysncInterval = 1, unsigned int flags = 0);

		void Discard();

	private:

		HRESULT CreateSwapChain(CommandQueue& commandQueue, Factory& factory, Window& window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum);

		HRESULT CreateRenderTargets(unsigned int bufferNum = 2);

	};

};