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

		//！インスタンス
		inline static Framework* _instance = nullptr;

		//!ウインドウ幅
		unsigned int _windowWidth;

		//！ウインドウ高さ
		unsigned int _windowHeight;

		//！バックバッファの数
		unsigned int _backBufferNum;

		//！WARPデバイスを使用するかどうか
		bool _useWarpDevice;

		//！APPクラスネーム
		std::wstring _appClassName;

		//!デバイス
		D3D12Device _device;

		//!デフォルトのコマンドリスト
		std::shared_ptr<CommandList> _defaultCommandList;

		//ファクトリ
		Factory _factory;

		//ウインドウ
		Window		_window;

		//!レンダリングマネージャ
		RenderingManager _renderingManager;

		//!入力管理マネージャ
		InputManager _inputManager;

		//！サウンド管理マネージャ
		AudioManager _audioManager;

		//!大麻
		Timer	_timer;

		//!マスター描画コマンドキュー
		CommandQueue _drawQueue;
		
		//!マスターコピーキュー
		CommandQueue _copyQueue;

		//!マスター計算コマンドキュー
		CommandQueue _computeQueue;

		enum DRAW_QUEUE_INDEX {
			DRAW_QUEUE_INDEX_ONE,
			DRAW_QUEUE_INDEX_TWO,
			DRAW_QUEUE_INDEX_THREE,
		};

	private:

	public:

		~Framework();

		/**
		* @fn
		* @brief インスタンスのセットアップ
		*/
		static void SetUp() {

			if (_instance == nullptr) {
				_instance = new Framework();
			}
		};

		/**
		* @fn
		* @brief インスタンス削除
		*/
		static void Destory() {

			if (_instance != nullptr) {
				delete _instance;
			}
		};

		/**
		* @fn
		* @brief イニシャライズ
		* @return 成功でS_OK
		*/
		static HRESULT Initialize();

		/**
		* @fn
		* @brief 終端処理
		*/
		static void Terminate();

		/**
		* @fn
		* @brief デバイスフェッチ
		* @return デバイス
		*/
		static K3D::D3D12Device& GetDevice();

		/**
		* @fn
		* @brief ファクトリフェッチ
		* @return ファクトリ
		*/
		static K3D::Factory& GetFactory();

		static K3D::CommandQueue& GetCommandQueue();

		static std::shared_ptr<CommandList> GetCommandList();

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

		HRESULT InitCommandList();

		HRESULT InitRenderingManager();

		HRESULT InitInputManager();

		HRESULT InitAudioManager();

		void TermWindow();

		void TermD3D();
	};
}

