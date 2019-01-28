#pragma once
namespace K3D {
	class MeshHeap;
	class MeshBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class GeometryState;

	class ModelMesh
	{
	public:
		
		//基本メッシュ情報を保持したもの
		std::unique_ptr<MeshBuffer> _meshBuffer;
		//デスクリプタヒープやマテリアルバッファ、テクスチャへの参照をもったもの
		std::unique_ptr<MeshHeap> _meshHeap;


	public:
		ModelMesh();
		~ModelMesh();
	};
}