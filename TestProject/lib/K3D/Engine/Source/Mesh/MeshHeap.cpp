#include "MeshHeap.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Texture/TextureObject.h"

K3D::MeshHeap::MeshHeap()  :
	_textureStartPoint(0),
	_materialStartPoint(0)
{
	_descriptors.clear();
	_descriptors.shrink_to_fit();
}


K3D::MeshHeap::~MeshHeap()
{
	Discard();
}


void K3D::MeshHeap::SetTransformDescStartIndex(unsigned int startIndex)
{
	this->_transformStartPoint = startIndex;

}

void K3D::MeshHeap::SetMaterialDescStartIndex(unsigned int startIndex)
{
	this->_materialStartPoint = startIndex;

}

void K3D::MeshHeap::SetTextureDescStartIndex(unsigned int startIndex)
{
	this->_textureStartPoint = startIndex;

}

unsigned int K3D::MeshHeap::GetTransformDescStartIndex()
{
	return _transformStartPoint;
}

unsigned int K3D::MeshHeap::GetTextureDescStartIndex()
{
	return _textureStartPoint;
}

unsigned int K3D::MeshHeap::GetMaterialDescStartIndex()
{
	return _materialStartPoint;
}

K3D::ConstantBuffer & K3D::MeshHeap::GetMaterialBufffer()
{
	return this->_materialBuffer;
}

std::vector<std::weak_ptr<K3D::TextureObject>>& K3D::MeshHeap::GetTexturesRef()
{
	return this->_textureResource;
}

std::weak_ptr<K3D::TextureObject> K3D::MeshHeap::GetTexturteRef(unsigned int textureIndex)
{
	if (_textureResource.size() == 0 || _textureResource.size() <= textureIndex) {
		return std::weak_ptr<K3D::TextureObject>();
	}
	return _textureResource[textureIndex];
}

K3D::MeshHeap & K3D::MeshHeap::AddTextureRef(std::weak_ptr<K3D::TextureObject> textureRef)
{
	this->_textureResource.push_back(textureRef);
	return *this;
}

std::vector<std::weak_ptr<K3D::Descriptor>>& K3D::MeshHeap::GetDescriptors()
{
	return  _descriptors;
}

void K3D::MeshHeap::AddDescriptor(std::weak_ptr<Descriptor>& descriptor)
{
	_descriptors.push_back(descriptor);
}

void K3D::MeshHeap::Discard()
{
	_descriptors.clear();
	_descriptors.shrink_to_fit();
	_materialBuffer.Discard();
	_textureResource.clear();
	_textureResource.shrink_to_fit();
}
