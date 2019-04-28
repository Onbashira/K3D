#include "GeometryState.h"

K3D::GeometryState::GeometryState() :
	_vertexBuffers(), _indexBuffer()
{

	_indexBuffer.BufferLocation = 0;
	_indexBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	_indexBuffer.SizeInBytes = 0;

}

K3D::GeometryState::~GeometryState()
{

}

const std::vector<D3D12_VERTEX_BUFFER_VIEW>* K3D::GeometryState::GetVertexBufferView()
{
	return &_vertexBuffers;
}

const D3D12_INDEX_BUFFER_VIEW * K3D::GeometryState::GetIndexBufferView()
{
	return &_indexBuffer;
}

void K3D::GeometryState::AddVertexBufferView(D3D12_VERTEX_BUFFER_VIEW && vertexBufferView)
{
	this->_vertexBuffers.push_back(std::move(vertexBufferView));

}

void K3D::GeometryState::AddVertexBufferView(const D3D12_VERTEX_BUFFER_VIEW & vertexBufferView)
{
	this->_vertexBuffers.push_back(vertexBufferView);
}

void K3D::GeometryState::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW & indexBufferView)
{
	this->_indexBuffer = indexBufferView;
}
