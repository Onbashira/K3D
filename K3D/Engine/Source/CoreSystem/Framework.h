#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Window/Window.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Factory/Factory.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Time/Timer.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/TemporarilyBuffer/TemporarilyBuffer.h"
#include "Engine/Source/Input/InputManager.h"
#include "Engine/Source/Audio/AudioManager.h"
#include <string>
#include <vector>

namespace K3D {

	class Framework : NonCopyable
	{

	public:

		inline static Framework* _instance = nullptr;

		unsigned int _windowWidth;

		unsigned int _windowHeight;

		unsigned int _backBufferNum;

		bool _useWarpDevice;

		std::wstring _appClassName;

		D3D12Device _device;

		CommandQueue _queue;

		Factory _factory;

		Window		_window;

		RenderingManager _renderingManager;

		InputManager _inputManager;

		AudioManager _audioManager;

		Timer	_timer;

		std::vector<CommandList*> _commandListTempBuffer;

		std::vector<CommandQueue> _drawQueue;

		std::vector<CommandQueue> _computeQueue;

		enum DRAW_QUEUE_INDEX {
			DRAW_QUEUE_INDEX_ONE,
			DRAW_QUEUE_INDEX_TWO,
			DRAW_QUEUE_INDEX_THREE,
		};

	private:

	public:

		~Framework();

		static void SetUp() {
		
			if (_instance == nullptr) {
				_instance = new Framework();
			}
		};


		static void Destory() {

			if (_instance != nullptr) {
				delete _instance;
			}
		};

		static HRESULT Initialize();

		static void Terminate();

		static K3D::D3D12Device& GetDevice();

		static K3D::Factory& GetFactory();

		static K3D::CommandQueue& GetCommandQueue();

		static K3D::Window& GetWindow();

		static K3D::RenderingManager& GetRenderingManagre();

		static K3D::Timer& Time();

		static void FlipScreen();

		static void ClearScreen(std::weak_ptr<CommandList> list);

		static void SetWindowName(std::wstring name);

		static void SetWindowSize(unsigned int width, unsigned int  height);

		static void SetBackBufferNum(unsigned int backBufferNum);

		static void TimerStart();

		static void TimerStop();

		static void Tick();

		static float DeltaTime();

		static InputManager& Input();

		static AudioManager& AudioManager();

		static int LoopMessage();

		static void PushBackOntimeBuffer(Resource* buffer);

		static void PushBackOntimeBuffer(CommandList* buffer);

		static void PushBackOntimeBuffer(DescriptorHeap* buffer);

		static void PushBackOntimeBuffer(Microsoft::WRL::ComPtr<ID3D12DeviceChild> buffer);

		static void PushFrontCommandList(CommandList* drawlist);

		static void PushBackCommandList(CommandList* resourcelist);

	private:

		Framework();

		Framework(const Framework&) {};

		Framework(Framework&&) {};

		Framework& operator =(const Framework&) { return *this; };

		Framework& operator =(Framework&&) { return *this; };

		HRESULT InitWindow();

		HRESULT InitD3D12();

		HRESULT InitDevice();

		HRESULT InitFactory();

		HRESULT InitCommandQueue();

		HRESULT InitRenderingManager();

		HRESULT InitInputManager();

		HRESULT InitAudioManager();

		void TermWindow();

		void TermD3D();
	};
}

