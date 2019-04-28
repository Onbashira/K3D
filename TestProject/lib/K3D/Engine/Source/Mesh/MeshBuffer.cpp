#include "MeshBuffer.h"
#include "Engine/Source/Resource/VertexBuffer.h"
#include "Engine/Source/Resource/IndexBuffer.h"

K3D::MeshBuffer::MeshBuffer()
{
}

K3D::MeshBuffer::~MeshBuffer()
{
}

void K3D::MeshBuffer::InitializeVBO(ULONG64 size, unsigned int stride, void * vertexDataSrc)
{
	this->_mesh.CreateVBO(size, stride, vertexDataSrc);
}

void K3D::MeshBuffer::AddCustomVBO(ULONG64 size, unsigned int stride, void * customVertexDataSrc)
{
	std::unique_ptr<VertexBuffer> buffer = std::make_unique<K3D::VertexBuffer>();
	buffer->Initialize(size, stride, customVertexDataSrc);
	this->_additionalVBOs.push_back(std::move(buffer));
}

void K3D::MeshBuffer::InitializeIBO(std::vector<unsigned int>& indexListDataSrc)
{
	this->_mesh.CreateIBO(indexListDataSrc.size() * sizeof(unsigned int), sizeof(unsigned int), indexListDataSrc.data());
}

std::vector<D3D12_VERTEX_BUFFER_VIEW> K3D::MeshBuffer::GetMeshVBViews()
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> views;
	auto& geometryState = _mesh.GetGeometryState().GetVertexBufferView();
	
	if (geometryState.SizeInBytes != 0) {
		views.push_back(geometryState);
	}
	unsigned int cycleNum = static_cast<unsigned int>(this->_additionalVBOs.size());
	for (unsigned int i = 0;i < cycleNum; ++i) {
		views.push_back(this->_additionalVBOs[i]->GetView());
	}
	return views;
}

D3D12_INDEX_BUFFER_VIEW K3D::MeshBuffer::GetMeshIBOView()
{
	return _mesh.GetGeometryState().GetIndexBufferView();
}

std::unique_ptr<K3D::VertexBuffer>& K3D::MeshBuffer::GetVBO()
{
	return _mesh.GetVBO();
}

std::unique_ptr<K3D::VertexBuffer>& K3D::MeshBuffer::GetCustomVBO(unsigned int index)
{
	if (this->_additionalVBOs.empty() || this->_additionalVBOs.size() <= index) {
		//テンポラリオブジェクト
		return std::unique_ptr<K3D::VertexBuffer>(nullptr);
	}
	return this->_additionalVBOs[index];
}

std::unique_ptr<K3D::IndexBuffer>& K3D::MeshBuffer::GetIBO()
{
	return _mesh.GetIBO();
}

void K3D::MeshBuffer::Discard()
{
	this->_mesh.Discard();
	this->_additionalVBOs.clear();
	this->_additionalVBOs.shrink_to_fit();
}
