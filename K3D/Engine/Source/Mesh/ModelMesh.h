#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace K3D {
	class MeshHeap;
	class MeshBuffer;

	class ModelMesh
	{

	public:
		//��{���b�V������ێ���������
		std::unique_ptr<MeshBuffer> meshBuffer;
		//�f�X�N���v�^�q�[�v��}�e���A���o�b�t�@�A�e�N�X�`���ւ̎Q�Ƃ�����������
		std::unique_ptr<MeshHeap> meshHeap;

		GeometryState GetGeometryState();

		UINT64 GetIndexListCount();

		ModelMesh();

		~ModelMesh();

	};
}