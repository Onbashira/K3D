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
		* @param[in] heap �q�[�v�̃e�N�X�`���r���[�̃X�^�[�g�C���f�b�N�X
		* @param[in] modelName ���f���̖��O
		* @param[in] paths �t�@�C���p�X
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

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
		*/
		HRESULT LoadUpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource,D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		HRESULT LoadWriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path );

		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		bool IsUseGamma(DXGI_FORMAT format);

		HRESULT LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, std::string path);
		
	};
}