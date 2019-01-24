#include "stdafx.h"
#include "TextureObject.h"
#include "Engine/Source/Texture/TextureLoader.h"

K3D::TextureObject::TextureObject()
{
}

K3D::TextureObject::TextureObject(std::shared_ptr<ShaderResource> sr, const TextureObjectDesc & desc):
	_textureResource(sr),_desc(desc)
{
}

K3D::TextureObject::~TextureObject()
{
}

const K3D::TextureObjectDesc & K3D::TextureObject::GetDesc() const
{
	return _desc;
}

void K3D::TextureObject::SetDesc(TextureObjectDesc && desc)
{
	this->_desc = desc;
}

void K3D::TextureObject::LoadTexture(std::string texturePath) const
{
	TextureLoader::GetInstance().LoadTexture(texturePath);
}

std::weak_ptr<K3D::ShaderResource> K3D::TextureObject::GetShaderResource()
{
	return this->_textureResource;
}
