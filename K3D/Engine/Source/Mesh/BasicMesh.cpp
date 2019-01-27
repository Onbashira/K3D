#include "BasicMesh.h"
#include "Engine/Source/Resource/IndexBuffer.h"
#include "Engine/Source/Resource/VertexBuffer.h"

void K3D::BasicMesh::CreateVBO(ULONG64 size, UINT stride, const void * pVertices)
{
	this->_meshVBO = std::make_unique<VertexBuffer>();
	_meshVBO->Create(size, stride, pVertices);
	this->_geometry.SetVertexBufferView(_meshVBO->GetView());
}

void K3D::BasicMesh::CreateIBO(ULONG64 size, size_t elementSize, const void * pIndices)
{
	this->_meshIBO = std::make_unique<IndexBuffer>();
	_meshIBO->Create(size, elementSize, pIndices);
	this->_geometry.SetIndexBufferView(_meshIBO->GetView());
}

void K3D::BasicMesh::SetMeshID(Uint32 id)
{
	this->_meshID = id;
}

std::unique_ptr<K3D::VertexBuffer>& K3D::BasicMesh::GetVBO()
{
	return this->_meshVBO;
}

std::unique_ptr<K3D::IndexBuffer>& K3D::BasicMesh::GetIBO()
{
	return this->_meshIBO;
}

K3D::GeometryState & K3D::BasicMesh::GetGeometryState()
{
	return _geometry;
}

unsigned int K3D::BasicMesh::GetID()
{
	return _meshID;
}

void K3D::BasicMesh::Discard()
{

	this->_meshIBO->Discard();
	this->_meshIBO.release();
	this->_meshVBO->Discard();
	this->_meshVBO.release();
	this->_meshID = 0;
}
