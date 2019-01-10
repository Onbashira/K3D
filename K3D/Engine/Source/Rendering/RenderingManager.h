#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Rendering/SwapChain.h"
namespace K3D {

	class CommandQueue;
	class Factory;
	class Window;

	class RenderingManager : private NonCopyable
	{

		friend class Framework;

	public:

	private:

		SwapChain _swapChain;

	public:

		~RenderingManager();

		HRESULT Initialize(CommandQueue* commandQueue, Factory* factory, Window* window, UINT windowWidth, UINT windowHeight, unsigned int bufferNum = 2);

		void ClearScreen(std::weak_ptr<CommandList> list);

		void FlipScreen();

		void CopyToRenderTarget(std::weak_ptr<CommandList> list, Resource* buffer);

		void BeginRendering();

		void EndRendering();

		void RenderingUpdate();

		void Term();

	private:

		RenderingManager() {};

		RenderingManager(const RenderingManager&) {};

		RenderingManager(RenderingManager&&) {};

		RenderingManager& operator =(const RenderingManager&) { return *this; };

		RenderingManager& operator =(RenderingManager&&) { return *this; };

	};
}
