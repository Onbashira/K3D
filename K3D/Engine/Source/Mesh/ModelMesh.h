#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace K3D {
	class MeshHeap;
	class MeshBuffer;

	class ModelMesh
	{

	public:

	private:
		//��{���b�V������ێ���������
		std::unique_ptr<MeshBuffer> _meshBuffer;
		//�f�X�N���v�^�q�[�v��}�e���A���o�b�t�@�A�e�N�X�`���ւ̎Q�Ƃ����������� (�}�e���A���o�b�t�@�͕������ׂ����H�@���܂�ӎ����ĂȂ��������Ǖ��������ق����s����������������Ȃ�)
		std::unique_ptr<MeshHeap> _meshHeap;
	public:	
		
		ModelMesh();

		~ModelMesh();

		void InitializeIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		void AddVertexBuffer(size_t elementSize, size_t elementNum, void * pSrc);

		const std::unique_ptr<K3D::GeometryState>& GetGeometryState();

		const std::unique_ptr < K3D::IndexBuffer > & GetIndexBuffer();

		std::vector<std::weak_ptr<K3D::TextureObject>>& GetTexturesRef();

		std::weak_ptr<K3D::TextureObject> GetTextureRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<K3D::TextureObject> textureRef);

		std::vector <std::weak_ptr<Descriptor>>& GetDescriptors();

		void AddDescriptor(std::weak_ptr<Descriptor>& descriptor);

		void Discard();

	};
}