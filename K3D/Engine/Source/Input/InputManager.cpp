#include "InputManager.h"


K3D::InputManager::InputManager()  :
	_mousePos({})
{
}


K3D::InputManager::~InputManager()
{
}


void K3D::InputManager::InputUpdate()
{
	CopyMemory(_preKey,_key,sizeof(_preKey));
	if (!GetKeyboardState(&_key[0])) {
		MessageBox(nullptr, L"ƒL[î•ñ‚Ìæ“¾‚É¸”s", L"ERROR", MB_OK);
		return ;
	}
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(_forcusWindowHandle, &pos);
#ifdef _DEBUG
	//std::cout <<pos.x << std::endl;
	//std::cout <<pos.y << std::endl;
#endif // _DEBUG
	_mousePos.x = float(pos.x);
	_mousePos.x = float(pos.y);

	_gamePadManager.Update();

}

bool K3D::InputManager::IsTriggerDown(VIRTUAL_KEY_STATE code)
{
	if (!((_preKey[code] & (0x80)) == 0x80) && ((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool K3D::InputManager::IsTriggerUp(VIRTUAL_KEY_STATE code)
{
	if (((_preKey[code] & (0x80)) == 0x80) && !((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool K3D::InputManager::IsToggle(VIRTUAL_KEY_STATE code)
{
	if ((_key[code] & (0x01f)) == (0x01)) {
		return true;
	};
	return false;
}

Vector2 K3D::InputManager::GetMousePos()
{
	return _mousePos;
}


bool K3D::InputManager::IsDown(K3D::VIRTUAL_KEY_STATE code)
{

	if (_key[code] & 0x80) {
		return true;
	}
	return false;
}

void K3D::InputManager::Discard()
{
	this->_forcusWindowHandle = nullptr;
}

void K3D::InputManager::SetFocusWindow(HWND forcusWindow)
{
	_forcusWindowHandle = forcusWindow;
}

void K3D::InputManager::GamePadInitialize(int cotrollerNum)
{
	_gamePadManager.Initialize(cotrollerNum);
}

K3D::GamePadManager & K3D::InputManager::GetGamePadManager()
{
	return _gamePadManager;
}

std::shared_ptr<K3D::GamePad> K3D::InputManager::GetGamePad(int controllerID)
{
	return _gamePadManager.GetPad(controllerID);
}
