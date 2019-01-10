#include "GamePadManager.h"
#include "GamePad.h"
#include "Engine/Source/Utility/Utility.h"
constexpr int MaxConectingCount = 4;

void K3D::GamePadManager::AllSetUp()
{
	for (int i = 0; i < _maxConectionCount;i++) {
		this->_gamePads.push_back(std::move(std::make_shared<GamePad>()));
		this->_gamePads[i]->SetUp(i);
	}
}

void K3D::GamePadManager::Update()
{
	for (size_t i = 0; i < _gamePads.size(); i++) {
		this->_gamePads[i]->Update();
	}
}

std::shared_ptr<K3D::GamePad> K3D::GamePadManager::GetPad(int padID)
{
	return _gamePads[::Clamp(padID, 0, MaxConectingCount-1)];
}

void K3D::GamePadManager::Initialize(int maxConectionCount)
{
	this->_maxConectionCount = ::Clamp(maxConectionCount,0, MaxConectingCount);
	AllSetUp();
}

void K3D::GamePadManager::Discard()
{
	for (int i = 0; i < _maxConectionCount; i++) {
		this->_gamePads[i].reset();
	}
	
}

K3D::GamePadManager::GamePadManager() : _maxConectionCount(0)
{
}

K3D::GamePadManager::~GamePadManager()
{
	Discard();
}
