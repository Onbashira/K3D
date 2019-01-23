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
		* @brief モデルのテクスチャのロード
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] modelName モデルの名前
		* @param[in] paths ファイルパス
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::string modelName, std::vector<std::string>& paths);

		/**
		* @fn
		* @brief モデルのテクスチャのロードとヒープへビューを作成
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] heap ヒープの参照
		* @param[in] heap ヒープのテクスチャビューのスタートインデックス
		* @param[in] modelName モデルの名前
		* @param[in] paths ファイルパス
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

	private:

		TextureLoader();

		/**
		* @fn
		* @brief UpdateSubresourceを用いたバッファへの書き込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] resource シェーダーリソースへの弱参照
		* @param[out] subResource サブリソース
		* @param[in] modelName パス
		* @param[in] paths ファイルパス
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