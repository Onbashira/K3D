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
		
		//!テクスチャリソースへの参照
		std::shared_ptr<ShaderResource> _textureResource;
		
		//!デスクリプション
		TextureObjectDesc _desc;
		
	public:

		TextureObject();

		/**
		* @fn
		* @brief テクスチャオブジェクトの作成
		* @param[in] sr シェーダリソース
		* @param[in] desc デスクリプション
		* @return リザルト
		*/
		TextureObject( std::shared_ptr<ShaderResource> sr,const TextureObjectDesc& desc);

		~TextureObject();

		/**
		* @fn
		* @brief デスクリプションのフェッチ
		* @return デスクリプション
		*/
		const TextureObjectDesc& GetDesc()const;

		/**
		* @fn
		* @brief デスクリプションのフェッチ
		* @param[in] texturePath プロジェクトディレクトリ相対のテスクチャへのパス
		*/
		void LoadTexture(std::string texturePath)const;

	private:
	
	};

}