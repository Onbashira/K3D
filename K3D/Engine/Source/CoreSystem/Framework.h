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

		//�I�C���X�^���X
		inline static Framework* _instance = nullptr;

		//!�E�C���h�E��
		unsigned int _windowWidth;

		//�I�E�C���h�E����
		unsigned int _windowHeight;

		//�I�o�b�N�o�b�t�@�̐�
		unsigned int _backBufferNum;

		//�IWARP�f�o�C�X���g�p���邩�ǂ���
		bool _useWarpDevice;

		//�IAPP�N���X�l�[��
		std::wstring _appClassName;

		//!�f�o�C�X
		D3D12Device _device;

		//!�f�t�H���g�̃R�}���h���X�g
		std::shared_ptr<CommandList> _defaultCommandList;

		//�t�@�N�g��
		Factory _factory;

		//�E�C���h�E
		Window		_window;

		//!�����_�����O�}�l�[�W��
		RenderingManager _renderingManager;

		//!���͊Ǘ��}�l�[�W��
		InputManager _inputManager;

		//�I�T�E���h�Ǘ��}�l�[�W��
		AudioManager _audioManager;

		//!�喃
		Timer	_timer;

		//!�}�X�^�[�`��R�}���h�L���[
		CommandQueue _drawQueue;
		
		//!�}�X�^�[�R�s�[�L���[
		CommandQueue _copyQueue;

		//!�}�X�^�[�v�Z�R�}���h�L���[
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
		* @brief �C���X�^���X�̃Z�b�g�A�b�v
		*/
		static void SetUp() {

			if (_instance == nullptr) {
				_instance = new Framework();
			}
		};

		/**
		* @fn
		* @brief �C���X�^���X�폜
		*/
		static void Destory() {

			if (_instance != nullptr) {
				delete _instance;
			}
		};

		/**
		* @fn
		* @brief �C�j�V�����C�Y
		* @return ������S_OK
		*/
		static HRESULT Initialize();

		/**
		* @fn
		* @brief �I�[����
		*/
		static void Terminate();

		/**
		* @fn
		* @brief �f�o�C�X�t�F�b�`
		* @return �f�o�C�X
		*/
		static K3D::D3D12Device& GetDevice();

		/**
		* @fn
		* @brief �t�@�N�g���t�F�b�`
		* @return �t�@�N�g��
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

