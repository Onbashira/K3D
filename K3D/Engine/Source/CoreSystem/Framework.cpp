#include "Framework.h"
#include "Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Texture/TextureManager.h"

K3D::Framework::Framework() :
	_windowWidth(640), _windowHeight(480), _useWarpDevice(false), _backBufferNum(3)
{

}

K3D::Framework::~Framework()
{

}


HRESULT K3D::Framework::Initialize()
{

	CHECK_RESULT(_instance->InitWindow());
	CHECK_RESULT(_instance->InitD3D12());

	return S_OK;
}

void K3D::Framework::Terminate()
{
	_instance->TermD3D();
	_instance->TermWindow();
}

std::shared_ptr<K3D::D3D12Device> & K3D::Framework::GetDevice()
{
	return _instance->_device;
}

K3D::Factory & K3D::Framework::GetFactory()
{
	return _instance->_factory;
}

K3D::Window & K3D::Framework::GetWindow()
{
	return _instance->_window;
}

K3D::RenderingManager & K3D::Framework::GetRenderingManagre()
{
	return _instance->_renderingManager;
}

K3D::Timer & K3D::Framework::Time()
{
	return _instance->_timer;
}

void K3D::Framework::FlipScreen()
{
	return _instance->_renderingManager.FlipScreen();

}

void K3D::Framework::ClearScreen(std::weak_ptr<CommandList> list)
{

	return _instance->_renderingManager.ClearScreen(list);

}

void K3D::Framework::SetWindowName(std::wstring name)
{
	_instance->_appClassName = name;
}

void K3D::Framework::SetWindowSize(unsigned int width, unsigned int height)
{
	_instance->_windowHeight = height;
	_instance->_windowWidth = width;
}

void K3D::Framework::GetWindowSize(unsigned int & width, unsigned int & height)
{
	width = _windowWidth;
	height = _windowHeight;
}

void K3D::Framework::SetBackBufferNum(unsigned int backBufferNum)
{
	_instance->_backBufferNum = backBufferNum;
}

void K3D::Framework::TimerStart()
{
	_instance->_timer.Start();
}

void K3D::Framework::TimerStop()
{
	_instance->_timer.Stop();

}

void K3D::Framework::Tick()
{
	_instance->_timer.Tick();

}

float K3D::Framework::DeltaTime()
{
	return 	_instance->_timer.DeltaTime();
}

K3D::InputManager& K3D::Framework::Input()
{
	return _instance->_inputManager;
}

K3D::AudioManager & K3D::Framework::AudioManager()
{
	return _instance->_audioManager;
}

int K3D::Framework::LoopMessage()
{
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return -1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return 0;
		}
	}
}

HRESULT K3D::Framework::InitWindow()
{
	return _window.Create(_appClassName, _windowWidth, _windowHeight);
}

HRESULT K3D::Framework::InitD3D12()
{
	CHECK_RESULT(InitFactory());

	CHECK_RESULT(InitDevice());

	CHECK_RESULT(InitRenderingManager());

	CHECK_RESULT(InitInputManager());

	CHECK_RESULT(InitAudioManager());


	return S_OK;
}

HRESULT K3D::Framework::InitDevice()
{
	HRESULT ret = {};
	_device = std::make_shared<D3D12Device>();
	ret = _device->Initialize(&_factory, _useWarpDevice);

	if (ret) {

		Util::Comment(L"D3D12Device‚Ì‰Šú‰»‚ÉŽ¸”s");
		return ret;
	}

	return ret;
}
HRESULT K3D::Framework::InitFactory()
{
	return _factory.Initialize();
}


HRESULT K3D::Framework::InitRenderingManager()
{
	return _renderingManager.Initialize(_device, &_factory, &_window, _windowWidth, _windowHeight, _backBufferNum);
}

HRESULT K3D::Framework::InitInputManager()
{
	_inputManager.GamePadInitialize();
	_inputManager.SetFocusWindow(_window.GetWindowHandle());
	return S_OK;
}

HRESULT K3D::Framework::InitAudioManager()
{
	_audioManager.InitializeXAudio2();
	_audioManager.StartSoundEngine();
	return S_OK;
}

HRESULT K3D::Framework::InitTextureManager()
{
	TextureManager::GetInstance().SetRenderContext();
	return S_OK;
}

void K3D::Framework::TermWindow()
{
	_inputManager.Discard();
	_audioManager.StopSoundEngine();
	_audioManager.Discard();
	TextureManager::GetInstance().Discard();
	_window.Discard();
}

void K3D::Framework::TermD3D()
{
	_renderingManager.Term();
	this->_timer.Stop();
	this->_factory.Discard();
	this->_device.reset();
}