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

		//描画は現状ユーザまかせ
		//ヒープ一括化システムは動作確認済み
		//ライブラリの動作として、レンダリング自体が通ることも確認済み
		
		//TODO
		//・DXRの組み込み確認として、チュートリアルを移植する
		//・MHWのパーティクルシステムを模倣した（GCCの資料参照）GPUパーティクルレンダラの実装
		//　・テストのレンダラは実装ずみ

		K3D::Framework::GetInstance().Input().InputUpdate();
	}
	K3D::Framework::GetInstance().TimerStop();

	K3D::Framework::Destory();

}