#include "stdafx.h"
#include "ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"


K3D::ModelMesh::ModelMesh() :
	_meshBuffer(new MeshBuffer()), _meshHeap(new MeshHeap)
{

}

K3D::ModelMesh::~ModelMesh()
{
}

void K3D::ModelMesh::InitializeIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc)
{
	_meshBuffer->CreateIndexBuffer(elementSize, elementNum, pSrc);

}

void K3D::ModelMesh::AddVertexBuffer(size_t elementSize, size_t elementNum, void * pSrc)
{
	_meshBuffer->AddVertexBuffer(elementSize, elementNum, pSrc);
}


const std::unique_ptr<K3D::GeometryState>& K3D::ModelMesh::GetGeometryState()
{
	return  _meshBuffer->GetGeometryState();
}

const std::unique_ptr<K3D::IndexBuffer>& K3D::ModelMesh::GetIndexBuffer()
{
	return  _meshBuffer->GetIndexBuffer();

}

std::vector<std::weak_ptr<K3D::TextureObject>>& K3D::ModelMesh::GetTexturesRef()
{
	_meshHeap->GetTexturesRef();
}

std::weak_ptr<K3D::TextureObject> K3D::ModelMesh::GetTextureRef(unsigned int textureIndex)
{
	return _meshHeap->GetTextureRef(textureIndex);
}

K3D::MeshHeap & K3D::ModelMesh::AddTextureRef(std::weak_ptr<K3D::TextureObject> textureRef)
{
	_meshHeap->AddTextureRef(textureRef);
}

std::vector<std::weak_ptr<K3D::Descriptor>>& K3D::ModelMesh::GetDescriptors()
{
	_meshHeap->GetDescriptors();
}

void K3D::ModelMesh::AddDescriptor(std::weak_ptr<Descriptor>& descriptor)
{
	_meshHeap->AddDescriptor(descriptor);
}

void K3D::ModelMesh::Discard()
{
	_meshHeap->Discard();
	_meshBuffer->Discard();
}
