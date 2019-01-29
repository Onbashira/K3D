#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>
#include <vector>

namespace K3D {

	class ShaderResource;
	class TextureObject;
	class CommandList;

	//Mesh�������_�����O���邽�߂̃N���X�B��ɃV�F�[�_�[�ɓ����邽�߂̃q�[�v���Ǘ�����
	class MeshHeap
	{
		friend class ModelMesh;
	public:
	protected:
	private:

		//!�f�X�N���v�^�q�[�v (�ό`��� + �}�e���A���� + �e�N�X�`����)�@�J�����̏��͕ʂœ�����
		K3D::DescriptorHeap									_heap;
		//!�ό`���f�X�N���v�^�̊J�n�ʒu (maybe 0)
		unsigned int										_transformStartPoint;
		//!�}�e���A���f�X�N���v�^�̊J�n�ʒu (maybe 0+1 ~ x)
		unsigned int										_materialStartPoint;
		//!�e�N�X�`���f�X�N���v�^�̊J�n�ʒu (maybe x+1 ~ y)
		unsigned int										_textureStartPoint;
		//!�}�e���A�����̃o�b�t�@ �@�@
		K3D::ConstantBuffer									_materialBuffer;
		//!�e�N�X�`���ւ̎Q��
		std::vector<std::weak_ptr<K3D::TextureObject>>		_textureResource;


	public:

		MeshHeap();

		virtual ~MeshHeap();

		void BindingDescriptorHeaps(std::weak_ptr<K3D::CommandList> list);

		void BindingDescriptorHeaps(K3D::CommandList& lsit);

		void SetTransformDescStartIndex(unsigned int startIndex = 0);

		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		K3D::DescriptorHeap& GetHeap();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<K3D::TextureObject>>& GetTexturesRef();

		std::weak_ptr<K3D::TextureObject> GetTexturteRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<K3D::TextureObject> textureRef);

		void Discard();

	protected:

	private:

	};
}
