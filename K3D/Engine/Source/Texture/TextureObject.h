#pragma once
#include <d3d12.h>
#include <DirectXTex.h>

namespace K3D {

	class ShaderResource;

	struct TextureObjectDesc {
		float gamma;
		std::string fileName;
		D3D12_SUBRESOURCE_DATA subResource;
		DirectX::TexMetadata metaData;
	};

	class TextureObject
	{
	public:

	private:
		
		//!�e�N�X�`�����\�[�X�ւ̎Q��
		std::shared_ptr<ShaderResource> _textureResource;
		
		//!�f�X�N���v�V����
		TextureObjectDesc _desc;
		
	public:

		TextureObject();

		/**
		* @fn
		* @brief �e�N�X�`���I�u�W�F�N�g�̍쐬
		* @param[in] sr �V�F�[�_���\�[�X
		* @param[in] desc �f�X�N���v�V����
		* @return ���U���g
		*/
		TextureObject( std::shared_ptr<ShaderResource> sr,const TextureObjectDesc& desc);

		~TextureObject();

		/**
		* @fn
		* @brief �f�X�N���v�V�����̃t�F�b�`
		* @return �f�X�N���v�V����
		*/
		const TextureObjectDesc& GetDesc()const;

		/**
		* @fn
		* @brief �f�X�N���v�V�����̃t�F�b�`
		* @param[in] texturePath �v���W�F�N�g�f�B���N�g�����΂̃e�X�N�`���ւ̃p�X
		*/
		void LoadTexture(std::string texturePath)const;

	private:
	
	};

}