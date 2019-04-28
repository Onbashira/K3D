#include "K3D.h"
void main() {
	K3D::Framework::SetUp();
	K3D::Framework::GetInstance().SetBackBufferNum(2);
	K3D::Framework::GetInstance().SetWindowName(L"test");
	K3D::Framework::GetInstance().SetWindowSize(1280, 720);
	K3D::Framework::GetInstance().Initialize();
	K3D::Framework::GetInstance().TimerStart();


	while (K3D::Framework::GetInstance().LoopMessage() == 0) {
		K3D::Framework::GetInstance().Tick();
		K3D::Framework::GetInstance().Input().InputUpdate();
	}
	K3D::Framework::GetInstance().TimerStop();

	K3D::Framework::Destory();

}