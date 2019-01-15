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
		
		std::shared_ptr<ShaderResource> _textureResource;

		TextureObjectDesc _desc;
		
	public:

		TextureObject();

		TextureObject( std::shared_ptr<ShaderResource> sr,const TextureObjectDesc& desc);

		~TextureObject();

		const TextureObjectDesc& GetDesc()const;

	private:
	
	};

}