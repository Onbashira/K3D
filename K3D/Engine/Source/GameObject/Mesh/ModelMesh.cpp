#include <memory>
#include "ModelMesh.h"


K3D::ModelMesh::ModelMesh()
{
}


K3D::ModelMesh::~ModelMesh()
{
	DiscardMeshBuffer();
	DiscardMeshHeap();
}

void K3D::ModelMesh::BindVertexBufferToBundle()
{
	auto vertexBuffers = this->_meshBuffer.GetMeshVBViews();
	_bundleList.GetCommandList()->IASetVertexBuffers(0, vertexBuffers.size(), &vertexBuffers[0]);
}

void K3D::ModelMesh::BindIndexBufferToBundle()
{
	_bundleList.GetCommandList()->IASetIndexBuffer(&this->_meshBuffer.GetIBO().GetView());
}

K3D::MeshHeap & K3D::ModelMesh::GetMeshHeap()
{
	return _meshHeap;
}

K3D::MeshBuffer & K3D::ModelMesh::GetMeshBuffer()
{
	return _meshBuffer;
}

void K3D::ModelMesh::BindDescriptorHeaps(std::weak_ptr<K3D::GraphicsCommandList> list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D::ModelMesh::BindDescriptorHeaps(K3D::GraphicsCommandList & list)
{
	GetMeshHeap().BindingDescriptorHeaps(list);
}

void K3D::ModelMesh::DiscardMeshHeap()
{
	_meshHeap.GetHeap().Discard();
}

void K3D::ModelMesh::DiscardMeshBuffer()
{
	_meshBuffer.Discard();
}

