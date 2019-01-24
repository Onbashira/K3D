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
		* @param[in] heapStartIndex ヒープのテクスチャビューのスタートインデックス
		* @param[in] modelName モデルの名前
		* @param[in] paths ファイルパス
		*/
		void LoadModelTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, DescriptorHeap& heap, unsigned int heapStartIndex, std::string modelName, std::vector<std::string>& paths);

		/**
		* @fn
		* @brief テクスチャのロードを行う
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] paths ファイルパス
		* @return テクスチャオブジェクトの参照の所有権
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue,std::string texturePath);

		/**
		* @fn
		* @brief テクスチャのロードをマスターが持つコマンドキューとコマンドリストを用いて行う
		* @param[in] paths ファイルパス
		* @return テクスチャオブジェクトの参照の所有権
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::shared_ptr<D3D12Device>& device,std::string texturePath);

		/**
		* @fn
		* @brief テクスチャのロードをマスターが持つコマンドキューとコマンドリスト、デバイスを用いて行う
		* @param[in] paths ファイルパス
		* @return テクスチャオブジェクトの参照の所有権
		*/
		std::shared_ptr<TextureObject> LoadTexture(std::string texturePath);

		
		

		
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
		* @return リザルト
		*/
		HRESULT UpdateSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief WriteToSubResourceを用いたバッファへの書き込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[in] resource シェーダーリソースへの弱参照
		* @param[out] subResource サブリソース
		* @param[in] modelName パス
		* @param[in] paths ファイルパス
		* @return リザルト
		*/
		HRESULT WriteToSubResource(std::shared_ptr<CommandList> list, CommandQueue* commandQueue, std::weak_ptr<ShaderResource> resource, D3D12_SUBRESOURCE_DATA& subResource, std::string path);

		/**
		* @fn
		* @brief WICFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadWICFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief DDSFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadDDSFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief TGAFileの読み込み
		* @param[out] metaData メタデータへの参照
		* @param[out] scratchImage スクラッチデータへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadTGAFile(DirectX::TexMetadata& metaData, DirectX::ScratchImage& scratchImage, std::string& path);

		/**
		* @fn
		* @brief フォーマットからガンマ補正がかかっているかどうかの検討をつける
		* @param[in] format format
		* @return リザルト 真でガンマ値あり
		*/
		bool IsUseGamma(DXGI_FORMAT format);

		/**
		* @fn
		* @brief　テクスチャの読み込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[out] resource シェーダーリソースへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadRawTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::shared_ptr<ShaderResource>& resource, std::string path);

		/**
		* @fn
		* @brief テクスチャの読み込み
		* @param[in] commandList リストの参照
		* @param[in] commandQueue キューの参照
		* @param[out] textureObj テクスチャオブジェクトへの参照
		* @param[in] path ファイルパス
		* @return リザルト
		*/
		HRESULT LoadRawTexture(std::shared_ptr<CommandList> commandList, CommandQueue* commandQueue, std::shared_ptr<TextureObject>& textureObj, std::string path);



	};
}