#pragma once
namespace K3D {
	class MeshHeap;
	class MeshBuffer;

	struct ModelMesh
	{

		//基本メッシュ情報を保持したもの
		std::unique_ptr<MeshBuffer> meshBuffer;
		//デスクリプタヒープやマテリアルバッファ、テクスチャへの参照をもったもの
		std::unique_ptr<MeshHeap> meshHeap;

		ModelMesh();

		~ModelMesh();

	};
}