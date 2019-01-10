#pragma once
#include "Engine/Source/Resource/VertexBuffer.h"
#include "Engine/Source/Resource/IndexBuffer.h"
#include <d3d12.h>
#include <vector>
#include <memory>

namespace K3D {
	class VertexBuffer;
	class IndexBuffer;
	
	//Meshバッファを管理(CPUデータと分離する）
	class MeshBuffer
	{
	private:
		//SysMemに確保される頂点インデックスリスト
		IndexBuffer		_indexBuffer;
		//デフォルトで使用するVBO Stream0 PerVertex　デフォルトで頂点位置、法線、二次元UVテクスチャ座標をVBOにマップ?
		VertexBuffer	_vertexBuffer;
		//ユーザーカスタム型のVBO Stream1 PerVertex
		std::vector<VertexBuffer>	_customVBOs;
	
	public:

	private:

	protected:

	public:		
		
		virtual void InitializeVBO(ULONG64 size, unsigned int stride, void* vertexDataSrc);

		virtual void AddCustomVBO(ULONG64 size, unsigned int stride,void* customVertexDataSrc);

		virtual void InitializeIBO(std::vector<unsigned int>& indexListDataSrc);

		std::vector<D3D12_VERTEX_BUFFER_VIEW> GetMeshVBViews();

		VertexBuffer& GetVBO();

		VertexBuffer* GetCustomVBO(unsigned int index);

		IndexBuffer&  GetIBO();

		virtual void Discard();

		bool UseCustomVertex();

		MeshBuffer();
		virtual ~MeshBuffer();

	};

}