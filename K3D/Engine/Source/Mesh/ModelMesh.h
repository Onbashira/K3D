#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace K3D {
	class MeshHeap;
	class MeshBuffer;

	class ModelMesh
	{

	public:
		//基本メッシュ情報を保持したもの
		std::unique_ptr<MeshBuffer> meshBuffer;
		//デスクリプタヒープやマテリアルバッファ、テクスチャへの参照をもったもの
		std::unique_ptr<MeshHeap> meshHeap;

		GeometryState GetGeometryState();

		UINT64 GetIndexListCount();

		ModelMesh();

		~ModelMesh();

	};
}