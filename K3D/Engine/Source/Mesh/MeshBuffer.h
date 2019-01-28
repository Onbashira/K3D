#pragma once
#include "Engine/Source/Mesh/BasicMesh.h"
#include <vector>
#include <memory>

namespace K3D {

	class VertexBuffer;
	class IndexBuffer;

	//Meshバッファを管理(CPUデータと分離する）
	class MeshBuffer
	{

		friend class ModelMesh;

	public:

	protected:

		//!メッシュデータ
		BasicMesh _mesh;
		//ユーザーカスタム型のVBO Stream1 PerVertex
		std::vector<std::unique_ptr<VertexBuffer>>	_additionalVBOs;

	private:

	public:

		MeshBuffer();

		virtual ~MeshBuffer();

		virtual void InitializeVBO(ULONG64 size, unsigned int stride, void* vertexDataSrc);

		virtual void AddCustomVBO(ULONG64 size, unsigned int stride, void* customVertexDataSrc);

		virtual void InitializeIBO(std::vector<unsigned int>& indexListDataSrc);

		std::vector<D3D12_VERTEX_BUFFER_VIEW> GetMeshVBViews();

		D3D12_INDEX_BUFFER_VIEW GetMeshIBOView();

		std::unique_ptr<VertexBuffer>& GetVBO();

		std::unique_ptr<VertexBuffer>& GetCustomVBO(unsigned int index);

		std::unique_ptr<IndexBuffer>&  GetIBO();

		virtual void Discard();

	protected:

	private:

	};
}