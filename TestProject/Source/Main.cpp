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

		//�`��͌��󃆁[�U�܂���
		//�q�[�v�ꊇ���V�X�e���͓���m�F�ς�
		//���C�u�����̓���Ƃ��āA�����_�����O���̂��ʂ邱�Ƃ��m�F�ς�
		
		//TODO
		//�EDXR�̑g�ݍ��݊m�F�Ƃ��āA�`���[�g���A�����ڐA����
		//�EMHW�̃p�[�e�B�N���V�X�e����͕킵���iGCC�̎����Q�ƁjGPU�p�[�e�B�N�������_���̎���
		//�@�E�e�X�g�̃����_���͎�������

		K3D::Framework::GetInstance().Input().InputUpdate();
	}
	K3D::Framework::GetInstance().TimerStop();

	K3D::Framework::Destory();

}