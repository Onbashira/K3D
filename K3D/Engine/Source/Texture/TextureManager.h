#pragma once

#include"Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Utility/ManagerComponent.h"
#include <map>
#include <memory>


namespace K3D {

	class DescriptorHeap;
	class TextureLoader;
	class ShaderResource;

	class TextureManager
	{

	public:

		friend class TextureLoader;

	private:

		//ヌルテクスチャの名前
		const std::string _nullTextureName = "NULL_TEXTURE";

		//!黒テクスチャへの名前
		const std::string _nullTextureBlackName = "NULL_BLACK_TEXTURE";

		//!白テクスチャへの名前
		const std::string _nullTextureWhiteName = "NULL_WHITE_TEXTURE";

		//!情報キャリア
		UnorderedManagerComponent< ShaderResource> _textureResourceMap;


	public:

		~TextureManager();

		static TextureManager& GetInstance() {
			static TextureManager instance;
			return instance;
		};

		std::weak_ptr<ShaderResource> GetSpriteShaderResource(std::string name);

		std::weak_ptr<ShaderResource> GetModelTextureShaderResource(std::string modelPath);

		std::weak_ptr<ShaderResource> GetNullTextureShaderResource();

		std::weak_ptr<ShaderResource> GetNullBlackTextureShaderResource();

		std::weak_ptr<ShaderResource> GetNullWhiteTextureShaderResource();

		void Discard();

	private:

		TextureManager();

		TextureManager & operator=(const TextureManager& value) { return *this; };

		TextureManager & operator=(TextureManager&& value) { return *this; };

		TextureManager(const TextureManager& other) {};

		TextureManager(TextureManager&& other) {};
	};
}