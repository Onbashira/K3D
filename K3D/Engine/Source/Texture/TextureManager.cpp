#include "TextureManager.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../Resource/ShaderResource.h"


K3D::TextureManager::TextureManager()
{
	//ヌルテクスチャの作成
	std::shared_ptr<ShaderResource> resource = std::make_shared<ShaderResource>();
	{
		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
		props.CreationNodeMask = 0;
		props.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resDesc.Width = 16;
		resDesc.Height = 16;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 0;
		resDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resDesc.SampleDesc.Count = 1;
		resDesc.SampleDesc.Quality = 0;
		resDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

		resource->Create(16,16,1, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	
	}

	//参照の追加
	resource->SetName("Nulltexture ");
	_textureResourceMap.Set(this->_nullTextureName,resource);
	//参照の破棄
	resource.reset();

}


K3D::TextureManager::~TextureManager()
{
	Discard();
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureManager::GetSpriteShaderResource(std::string name)
{
	return _textureResourceMap.GetMap()[name];
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureManager::GetModelTextureShaderResource(std::string modelPath)
{
	return _textureResourceMap.GetMap()[modelPath];
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureManager::GetNullTextureShaderResource()
{
	return _textureResourceMap.GetMap()[this->_nullTextureName];
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureManager::GetNullBlackTextureShaderResource()
{
	return _textureResourceMap.GetMap()[this->_nullTextureBlackName];
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureManager::GetNullWhiteTextureShaderResource()
{
	return _textureResourceMap.GetMap()[this->_nullTextureWhiteName];
}

void K3D::TextureManager::Discard()
{
	_textureResourceMap.DiscardMap();
}


