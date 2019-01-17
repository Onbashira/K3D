#include "stdafx.h"
#include "TextureObject.h"


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
