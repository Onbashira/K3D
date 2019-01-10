#pragma once

#include <memory>
#include <vector>
#include <memory>

namespace K3D {

	//モデルのメッシュ描画に必要なクラス
	class ModelMesh : public DrawableComponent
	{
	private:
		MeshHeap _meshHeap;

		MeshBuffer _meshBuffer;
	protected:

	public:

	private:

	protected:

		virtual void BindVertexBufferToBundle();

		virtual void BindIndexBufferToBundle();
		//バンドルリストに継承先でドローコールバッチ化作業を強制
		virtual void RegisterToBundle()override = 0;

	public:

		//描画時呼び出し関数の作成を強制
		virtual void Draw()override = 0;

		MeshHeap& GetMeshHeap();

		MeshBuffer& GetMeshBuffer();

		void BindDescriptorHeaps(std::weak_ptr<K3D::GraphicsCommandList> list);

		void BindDescriptorHeaps(K3D::GraphicsCommandList & list);

		void DiscardMeshHeap();

		void DiscardMeshBuffer();

		ModelMesh();
		virtual ~ModelMesh();
	};
};

