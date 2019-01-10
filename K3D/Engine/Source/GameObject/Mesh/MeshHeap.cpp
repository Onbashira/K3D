#include "MeshHeap.h"
#include "Engine/Source/CommandList/CommandList.h"

K3D::MeshHeap::MeshHeap()  :
	_heap(),
	_textureStartPoint(0),
	_materialStartPoint(0)
{
}


K3D::MeshHeap::~MeshHeap()
{
	Discard();
}


void K3D::MeshHeap::BindingDescriptorHeaps(std::weak_ptr<K3D::CommandList> list)
{
	if (!list.expired()) {
		ID3D12DescriptorHeap* heaps[] = { this->_heap.GetPtr() };

		list.lock()->GetCommandList()->SetDescriptorHeaps(1, heaps);
	}
}

void K3D::MeshHeap::BindingDescriptorHeaps(K3D::CommandList & lsit)
{
	ID3D12DescriptorHeap* heaps[] = { this->_heap.GetPtr() };

	lsit.GetCommandList()->SetDescriptorHeaps(1, heaps);

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

K3D::DescriptorHeap & K3D::MeshHeap::GetHeap()
{
	return _heap;
}

K3D::ConstantBuffer & K3D::MeshHeap::GetMaterialBufffer()
{
	return this->_materialBuffer;
}

std::vector<std::weak_ptr<K3D::ShaderResource>>& K3D::MeshHeap::GetTexturesRef()
{
	return this->_textureResource;
}

std::weak_ptr<K3D::ShaderResource> K3D::MeshHeap::GetTexturteRef(unsigned int textureIndex)
{
	if (_textureResource.size() == 0 || _textureResource.size() <= textureIndex) {
		return std::weak_ptr<K3D::ShaderResource>();
	}
	return _textureResource[textureIndex];
}

K3D::MeshHeap & K3D::MeshHeap::AddTextureRef(std::weak_ptr<K3D::ShaderResource> textureRef)
{
	this->_textureResource.push_back(textureRef);
	return *this;
}


void K3D::MeshHeap::Discard()
{
	_heap.Discard();
	_materialBuffer.Discard();
	_textureResource.clear();
	_textureResource.shrink_to_fit();
}
