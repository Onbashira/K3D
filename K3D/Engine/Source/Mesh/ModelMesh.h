#pragma once
namespace K3D {
	class MeshHeap;
	class MeshBuffer;

	struct ModelMesh
	{

		//��{���b�V������ێ���������
		std::unique_ptr<MeshBuffer> meshBuffer;
		//�f�X�N���v�^�q�[�v��}�e���A���o�b�t�@�A�e�N�X�`���ւ̎Q�Ƃ�����������
		std::unique_ptr<MeshHeap> meshHeap;

		ModelMesh();

		~ModelMesh();

	};
}