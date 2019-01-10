#include "MeshBuffer.h"



K3D::MeshBuffer::MeshBuffer()
{

}


K3D::MeshBuffer::~MeshBuffer()
{
	Discard();
}


void K3D::MeshBuffer::InitializeVBO(ULONG64 size, unsigned int stride, void* vertexDataSrc)
{
	_vertexBuffer.Create(size, stride, vertexDataSrc);
}

void K3D::MeshBuffer::AddCustomVBO(ULONG64 size, unsigned int stride, void * customVertexDataSrc)
{
	_customVBOs.push_back(K3D::VertexBuffer());
	_customVBOs.back().Create(size, stride, customVertexDataSrc);
}

void K3D::MeshBuffer::InitializeIBO(std::vector<unsigned int>& indexListDataSrc)
{
	_indexBuffer.Create(sizeof(unsigned int)*indexListDataSrc.size(), sizeof(unsigned int), &indexListDataSrc[0]);
}

std::vector<D3D12_VERTEX_BUFFER_VIEW> K3D::MeshBuffer::GetMeshVBViews()
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> meshVBViews;

	meshVBViews.push_back(_vertexBuffer.GetView());
	if (this->_customVBOs.size() != 0) {
		for (auto& vb : _customVBOs) {
			meshVBViews.push_back(vb.GetView());
		}
	}
	return meshVBViews;
}

K3D::VertexBuffer& K3D::MeshBuffer::GetVBO()
{
	return _vertexBuffer;
}

K3D::VertexBuffer* K3D::MeshBuffer::GetCustomVBO(unsigned int index)
{
	if (this->_customVBOs.size() == 0 || (index >= _customVBOs.size())) {
		return nullptr;
	}
	return &_customVBOs[index];

}

K3D::IndexBuffer& K3D::MeshBuffer::GetIBO()
{
	return _indexBuffer;
}

void K3D::MeshBuffer::Discard()
{
	this->_customVBOs.clear();
	this->_vertexBuffer.Discard();
	this->_indexBuffer.Discard();

}

bool K3D::MeshBuffer::UseCustomVertex()
{
	return  this->_customVBOs.size() == 0 ? false : true;
}