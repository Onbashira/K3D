#include "TextureManager.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Resource/ShaderResource.h"
#include "Engine/Source/Texture/TextureObject.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Texture/TextureLoader.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"

K3D::TextureManager::TextureManager()
{
	CreateBlackTexture();
	CreateWhiteTexture();
}

K3D::TextureManager::~TextureManager()
{
	Discard();
}


void K3D::TextureManager::SetRenderContext(std::shared_ptr<RenderContext>& renderContext)
{
	_renderContext = renderContext;
}

std::shared_ptr<K3D::TextureObject> K3D::TextureManager::GetTexture(String filename)
{
	return _textureResourceMap.Get(filename).lock();
}

std::shared_ptr<K3D::TextureObject> K3D::TextureManager::LoadTexture(String filename)
{
	auto obj = TextureLoader::GetInstance().LoadTextureResource(_renderContext,filename);
	if (obj != nullptr)
	{
		this->_textureResourceMap.Set(filename, obj);
		return obj;
	}
	return obj;
}

std::shared_ptr<K3D::TextureObject> K3D::TextureManager::DuplicateTexture(String srcFilename, String dstFilename)
{
	return std::shared_ptr<TextureObject>();
}

std::shared_ptr<K3D::TextureObject> K3D::TextureManager::CreateColorTexture(String name, const Vector4 color)
{
	float maxValue = 255.0f;
	const unsigned int colorNum = 4;
	unsigned char uc_color[colorNum] = { static_cast<unsigned char>(color.x * maxValue),
		static_cast<unsigned char>(color.y * maxValue),
		static_cast<unsigned char>(color.z * maxValue),
		static_cast<unsigned char>(color.w * maxValue) };
	std::string texName = "SingleColorTexture";
	for (int i = 0; i < colorNum; ++i)
	{
		texName += Util::ConvertNumberToString(uc_color[i]);
	}

	auto findTex = _textureResourceMap.GetMap().find(texName);
	if (findTex != _textureResourceMap.GetMap().end())
	{
		return findTex->second;
	}

	const unsigned int width = 16;
	const unsigned int height = 16;
	std::vector<unsigned char> colorDatas(width * height * colorNum);
	for (unsigned int i = 0; i < static_cast<unsigned int>(colorDatas.size()); i += colorNum)
	{
		for (unsigned int j = 0; j < colorNum; ++j)
		{
			colorDatas[i + j] = uc_color[j];
		}
	}

	std::shared_ptr<TextureObject> obj = std::make_shared<TextureObject>();

	obj->_desc.subResource.pData = colorDatas.data();
	obj->_desc.subResource.RowPitch = width * sizeof(uc_color);
	obj->_desc.subResource.SlicePitch = obj->GetDesc().subResource.RowPitch * height;
	obj->_desc.fileName = texName;


	obj->_textureResource = std::make_shared<ShaderResource>();
	obj->_desc.gamma = 1.0f;
	TextureLoader::GetInstance().UpdateSubResource(_renderContext->GetResourceUpdateCmdList(K3D::RenderContext::RC_COMMAND_LIST_TYPE::BEGIN).lock(), 
		_renderContext,obj->_textureResource, obj->_desc.subResource,texName);
	_textureResourceMap.Set(name, obj);
	return obj;
}

std::weak_ptr<K3D::TextureObject> K3D::TextureManager::GetNullTexture()
{
	return _textureResourceMap.GetMap()[this->_nullTextureName];
}

std::weak_ptr<K3D::TextureObject> K3D::TextureManager::GetBlackTexture()
{
	return _textureResourceMap.GetMap()[this->_nullTextureBlackName];
}

std::weak_ptr<K3D::TextureObject> K3D::TextureManager::GetWhiteTexture()
{
	return _textureResourceMap.GetMap()[this->_nullTextureWhiteName];
}

void K3D::TextureManager::Discard()
{
	_textureResourceMap.DiscardMap();
}

void K3D::TextureManager::CreateBlackTexture()
{
	CreateColorTexture(this->_nullTextureBlackName, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void K3D::TextureManager::CreateWhiteTexture()
{
	CreateColorTexture(this->_nullTextureWhiteName, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

