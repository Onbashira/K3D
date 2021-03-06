#include "stdafx.h"
#include "MeshBuffer.h"

K3D::MeshBuffer::MeshBuffer() :
	_indexBuffer(std::make_unique<IndexBuffer>()),
	_geometryState(std::make_unique<GeometryState>())
{
}


K3D::MeshBuffer::~MeshBuffer()
{
}

void K3D::MeshBuffer::CreateIndexBuffer(size_t elementSize, size_t elementNum, void * pSrc)
{
	auto vRes = std::make_unique<VertexBuffer>();
	std::make_unique<VertexBuffer>()->Initialize(elementSize * elementNum, elementSize, pSrc);
	this->_vertexBuffers.push_back(std::move(vRes));
	
	D3D12_VERTEX_BUFFER_VIEW view{};
	view.BufferLocation = _vertexBuffers.back()->GetResource()->GetGPUVirtualAddress();
	view.SizeInBytes = elementSize * elementNum;
	view.StrideInBytes = elementSize;

	this->_geometryState->AddVertexBufferView(view);
}

void K3D::MeshBuffer::AddVertexBuffer( size_t elementSize, size_t elementNum, void * pSrc)
{
	this->_indexBuffer->Initialize(elementSize * elementNum, elementSize, pSrc);
	D3D12_INDEX_BUFFER_VIEW view;
	view.BufferLocation = _indexBuffer->GetResource()->GetGPUVirtualAddress();
	view.Format = _indexBuffer->GetResourceDesc().Format;
	view.SizeInBytes = elementSize * elementNum;

	_geometryState->SetIndexBufferView(view);
}

const std::unique_ptr < K3D::GeometryState > & K3D::MeshBuffer::GetGeometryState()
{
	return _geometryState;
}

const std::unique_ptr<K3D::IndexBuffer>& K3D::MeshBuffer::GetIndexBuffer()
{
	return _indexBuffer;
}

void K3D::MeshBuffer::Discard()
{
	_vertexBuffers.clear();
	_vertexBuffers.shrink_to_fit();
	_indexBuffer.reset();
	_geometryState.reset();
}
