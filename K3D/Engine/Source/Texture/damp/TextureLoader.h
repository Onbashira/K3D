#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <vector>
#include <string>
#include <DirectXTex.h>

namespace K3D {

	class ShaderResource;
	class CommandList;
	class CommandQueue;
	class D3D12Device;
	class TextureObject;

	class TextureLoader
	{
	private:

	public:

		~TextureLoader();

		static TextureLoader & GetInstance() {
			static TextureLoader loader;
			return loader;

		}

		/**
		* @fn
		* @brief ���f���̃e�N�X�`���̃��[�h
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] modelName ���f���̖��O
		* @param[in] paths �t�@�C���p�X
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::string modelName, std::vector<std::string>& paths);

		/**
		* @fn
		* @brief ���f���̃e�N�X�`���̃��[�h�ƃq�[�v�փr���[���쐬
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] heap �q�[�v�̎Q��
		* @param[in] heapStartIndex �q�[�v�̃e�N�X�`���r���[�̃X�^�[�g�C���f�b�N�X
		* @param[in] modelName ���f���̖��O
		* @param[in] paths �t�@�C���p�X
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

		/**
		* @fn
		* @brief �e�N�X�`���̃��[�h���s��
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] paths �t�@�C���p�X
		* @return �e�N�X�`���I�u�W�F�N�g�̎Q�Ƃ̏��L��
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue,std::string texturePath);

		/**
		* @fn
		* @brief �e�N�X�`���̃��[�h���}�X�^�[�����R�}���h�L���[�ƃR�}���h���X�g��p���čs��
		* @param[in] paths �t�@�C���p�X
		* @return �e�N�X�`���I�u�W�F�N�g�̎Q�Ƃ̏��L��
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::shared_ptr<D3D12Device>& device,std::string texturePath);

		/**
		* @fn
		* @brief �e�N�X�`���̃��[�h���}�X�^�[�����R�}���h�L���[�ƃR�}���h���X�g�A�f�o�C�X��p���čs��
		* @param[in] paths �t�@�C���p�X
		* @return �e�N�X�`���I�u�W�F�N�g�̎Q�Ƃ̏��L��
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::string texturePath);

		
		

		
	private:

		TextureLoader();

		/**
		* @fn
		* @brief UpdateSubresource��p�����o�b�t�@�ւ̏�������
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] resource �V�F�[�_�[���\�[�X�ւ̎�Q��
		* @param[out] subResource �T�u���\�[�X
		* @param[in] modelName �p�X
		* @param[in] paths �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT UpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief WriteToSubResource��p�����o�b�t�@�ւ̏�������
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[in] resource �V�F�[�_�[���\�[�X�ւ̎�Q��
		* @param[out] subResource �T�u���\�[�X
		* @param[in] modelName �p�X
		* @param[in] paths �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT WriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief WICFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief DDSFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief TGAFile�̓ǂݍ���
		* @param[out] metaData ���^�f�[�^�ւ̎Q��
		* @param[out] scratchImage �X�N���b�`�f�[�^�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief �t�H�[�}�b�g����K���}�␳���������Ă��邩�ǂ����̌���������
		* @param[in] format format
		* @return ���U���g �^�ŃK���}�l����
		*/
		bool IsUseGamma(DXGI_FORMAT format);

		/**
		* @fn
		* @brief�@�e�N�X�`���̓ǂݍ���
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[out] resource �V�F�[�_�[���\�[�X�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadRawTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::shared_ptr<ShaderResource>& resource, std::string path);

		/**
		* @fn
		* @brief �e�N�X�`���̓ǂݍ���
		* @param[in] commandList ���X�g�̎Q��
		* @param[in] commandQueue �L���[�̎Q��
		* @param[out] textureObj �e�N�X�`���I�u�W�F�N�g�ւ̎Q��
		* @param[in] path �t�@�C���p�X
		* @return ���U���g
		*/
		HRESULT LoadRawTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::shared_ptr<TextureObject>& textureObj, std::string path);



	};
}