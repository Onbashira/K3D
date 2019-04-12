#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Debug/Logger.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "TestScene.h"

void main() {
	K3D::Framework::SetUp();
	K3D::Framework::GetInstance().SetBackBufferNum(3);
	K3D::Framework::GetInstance().SetWindowName(L"test");
	K3D::Framework::GetInstance().SetWindowSize(640, 480);
	K3D::Framework::GetInstance().Initialize();
	K3D::Framework::GetInstance().TimerStart();

	std::shared_ptr<TestScene> testScene = std::shared_ptr<TestScene>(new TestScene());

	while (K3D::Framework::GetInstance().LoopMessage() == 0) {
		K3D::Framework::GetInstance().Tick();
		testScene->SceneBegin();
		testScene->Update();
		testScene->Rendering();
		testScene->SceneEnd();
	}
	K3D::Framework::GetInstance().TimerStop();

	K3D::Framework::Destory();

}