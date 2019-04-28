#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace K3D {

	class IndexBuffer;
	class VertexBuffer;

	//基本メッシュクラス
	class BasicMesh
	{
	public:

	protected:

		//!Meshの固有ID
		unsigned int _meshID;
		//!メッシュを構成するジオメトリステート
		GeometryState _geometry;
		//!ジオメトリリソース
		std::unique_ptr<VertexBuffer> _meshVBO;
		//!ジオメトリリソース
		std::unique_ptr<IndexBuffer> _meshIBO;

	private:

	public:

		BasicMesh() {};

		virtual ~BasicMesh() {};

		void CreateVBO(ULONG64 size, UINT stride, const void* pVertices);

		void CreateIBO(ULONG64 size, size_t elementSize, const void * pIndices);

		void SetMeshID(Uint32 id);

		std::unique_ptr<VertexBuffer>& GetVBO();

		std::unique_ptr<IndexBuffer>& GetIBO();

		GeometryState& GetGeometryState();

		unsigned int GetID();

		void Discard();

	private:
	
	};
}