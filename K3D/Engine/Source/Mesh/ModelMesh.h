#pragma once
namespace K3D {
	class MeshHeap;
	class MeshBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class GeometryState;

	class ModelMesh
	{
	public:
		
		//��{���b�V������ێ���������
		std::unique_ptr<MeshBuffer> _meshBuffer;
		//�f�X�N���v�^�q�[�v��}�e���A���o�b�t�@�A�e�N�X�`���ւ̎Q�Ƃ�����������
		std::unique_ptr<MeshHeap> _meshHeap;


	public:
		ModelMesh();
		~ModelMesh();
	};
}