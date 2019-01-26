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
		std::shared_ptr<D3D12Device> _device;

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
		void SetUp() {

			if (_instance == nullptr) {
				_instance = new Framework();
			}
		};

		/**
		* @fn
		* @brief インスタンス削除
		*/
		void Destory() {

			if (_instance != nullptr) {
				delete _instance;
			}
		};

		/**
		* @fn
		* @brief インスタンス削除
		*/
		static Framework& GetInstance() {

			if (_instance != nullptr) {
				return *_instance;
			}
			else {
				//強制エラー
				assert(0);
				std::exit(EXIT_FAILURE);
			}
		};


		/**
		* @fn
		* @brief イニシャライズ
		* @return 成功でS_OK
		*/
		HRESULT Initialize();

		/**
		* @fn
		* @brief 終端処理
		*/
		void Terminate();

		/**
		* @fn
		* @brief デバイスフェッチ
		* @return デバイス
		*/
		std::shared_ptr<K3D::D3D12Device>& GetDevice();

		/**
		* @fn
		* @brief ファクトリフェッチ
		* @return ファクトリ
		*/
		K3D::Factory& GetFactory();

		/**
		* @fn
		* @brief コマンドキューフェッチ
		* @return キュー
		*/
		K3D::CommandQueue& GetCommandQueue();

		/**
		* @fn
		* @brief コマンドリスト
		* @return リスト
		*/
		std::shared_ptr<CommandList> GetCommandList();

		/**
		* @fn
		* @brief ウインドウフェッチ
		* @return ウインドウ
		*/
		K3D::Window& GetWindow();

		/**
		* @fn
		* @brief 描画マネージャフェッチ
		* @return 描画マネージャ
		*/
		K3D::RenderingManager& GetRenderingManagre();

		/**
		* @fn
		* @brief タイマーフェッチ
		* @return タイマー
		*/
		K3D::Timer& Time();

		/**
		* @fn
		* @brief バックサーフェスをフリップする
		*/
		void FlipScreen();

		/**
		* @fn
		* @brief スクリーンをクリアする
		* @param[in] list　コマンドリストの弱参照
		*/
		void ClearScreen(std::weak_ptr<CommandList> list);

		/**
		* @fn
		* @brief ウインドウの名前を設定する
		* @param[in] name ウインドウの名前
		*/
		void SetWindowName(std::wstring name);

		/**
		* @fn
		* @brief ウインドウのサイズを設定する
		* @param[in] width ウインドウの幅
		* @param[in] height ウインドウの高さ
		*/
		void SetWindowSize(unsigned int width, unsigned int  height);

		/**
		* @fn
		* @brief バックバッファの数を設定する
		* @param[in] backBufferNum バックバッファの数
		*/
		void SetBackBufferNum(unsigned int backBufferNum);

		/**
		* @fn
		* @brief 計測開始
		*/
		void TimerStart();

		/**
		* @fn
		* @brief 計測停止
		*/
		void TimerStop();

		/**
		* @fn
		* @brief 呼び出し時点での差分をコミット
		*/
		void Tick();

		/**
		* @fn
		* @brief 前フレームとの差分を取得
		*/
		float DeltaTime();

		/**
		* @fn
		* @brief 入力マネージャフェッチ
		* @return 入力マネージャ
		*/
		InputManager& Input();

		/**
		* @fn
		* @brief オーディオマネージャフェッチ
		* @return オーディオマネージャ
		*/
		AudioManager& AudioManager();

		/**
		* @fn
		* @brief メッセージループ
		*/
		int LoopMessage();

	private:

		Framework();

		Framework(const Framework&) {};

		Framework(Framework&&) {};

		Framework& operator =(const Framework&) { return *this; };

		Framework& operator =(Framework&&) { return *this; };

		/**
		* @fn
		* @brief ウインドウの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitWindow();

		/**
		* @fn
		* @brief D3D12システムの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitD3D12();

		/**
		* @fn
		* @brief デバイスの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitDevice();

		/**
		* @fn
		* @brief ファクトリの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitFactory();

		/**
		* @fn
		* @brief コマンドキューの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitCommandQueue();

		/**
		* @fn
		* @brief コマンドリストの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitCommandList();

		/**
		* @fn
		* @brief レンダリングマネージャの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitRenderingManager();

		/**
		* @fn
		* @brief 入力マネージャの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitInputManager();

		/**
		* @fn
		* @brief オーディオマネージャの初期化
		* @return　リザルト　S_OKで成功
		*/
		HRESULT InitAudioManager();

		/**
		* @fn
		* @brief ウインドウの処理を終了する
		*/
		void TermWindow();

		/**
		* @fn
		* @brief D3D12システムを終了させる
		*/
		void TermD3D();
	};
}

