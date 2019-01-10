#pragma once
#include <memory>
#include <vector>


namespace K3D {
	class GamePad;

	class GamePadManager
	{
	public:

	private:
		//�V�X�e�����Ō��߂�ڑ��������Q�[���p�b�h�ő吔
		int _maxConectionCount;
		//���݂̐ڑ����ꂽ�Q�[���p�b�h
		std::vector<std::shared_ptr<GamePad>> _gamePads;
	public:
		//�e�p�b�h���A�b�v�f�[�g����
		void Update();
		//ID����Q�[���p�b�h���擾����
		std::shared_ptr<GamePad> GetPad(int padID = 0);
		//�p�b�h�S�̂̏�����
		void Initialize(int macConectionCount = 4);

		void Discard();

		GamePadManager();

		~GamePadManager();

	private:
		void AllSetUp();

	};
}
