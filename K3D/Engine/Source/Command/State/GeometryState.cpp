#include "GeometryState.h"

K3D::GeometryState::GeometryState() :
	_vertexBuffer(), _indexBuffer()
{

	_vertexBuffer.BufferLocation = 0;
	_vertexBuffer.SizeInBytes = 0;
	_vertexBuffer.StrideInBytes = 0;

	_indexBuffer.BufferLocation = 0;
	_indexBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	_indexBuffer.SizeInBytes = 0;

}

K3D::GeometryState::~GeometryState()
{

}

D3D12_VERTEX_BUFFER_VIEW & K3D::GeometryState::GetVertexBufferView()
{
	return this->_vertexBuffer;
}

D3D12_INDEX_BUFFER_VIEW & K3D::GeometryState::GetIndexBufferView()
{
	return this->_indexBuffer;
}

void K3D::GeometryState::SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW & vertexBufferView)
{
	this->_vertexBuffer = vertexBufferView;
}

void K3D::GeometryState::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW & indexBufferView)
{
	this->_indexBuffer = indexBufferView;
}
