#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Debug/Logger.h"
#include <string>
#include <sstream>
#include <iomanip>


void main() {
	K3D::Framework::SetUp();
	K3D::Framework::SetBackBufferNum(2);
	K3D::Framework::SetWindowName(L"test");
	K3D::Framework::SetWindowSize(1280, 720);
	K3D::Framework::Initialize();
	K3D::Framework::TimerStart();

	while (K3D::Framework::LoopMessage() == 0) {
		K3D::Framework::Tick();

	}
	K3D::Framework::TimerStop();

	K3D::Framework::Destory();

}