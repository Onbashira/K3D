#pragma once
#include "Engine/Source/DescriptorHeap/Descriptor.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>
#include <vector>

namespace K3D {

	class ShaderResource;
	class TextureObject;
	class CommandList;
	class ModelMesh;

	//Meshをレンダリングするためのクラス。主にシェーダーに投げるためのヒープを管理する
	class MeshHeap
	{
		friend class ModelMesh;
	public:
	protected:
	private:

		//!デスクリプタヒープ (変形情報 + マテリアル数 + テクスチャ数)　カメラの情報は別で投げる
		std::vector<std::shared_ptr<K3D::Descriptor>>		_descriptors;
		//!変形情報デスクリプタの開始位置 (maybe 0)
		unsigned int										_transformStartPoint;
		//!マテリアルデスクリプタの開始位置 (maybe 0+1 ~ x)
		unsigned int										_materialStartPoint;
		//!テクスチャデスクリプタの開始位置 (maybe x+1 ~ y)
		unsigned int										_textureStartPoint;
		//!マテリアル情報のバッファ 　　
		K3D::ConstantBuffer									_materialBuffer;
		//!テクスチャへの参照
		std::vector<std::weak_ptr<K3D::TextureObject>>		_textureResource;


	public:

		MeshHeap();

		virtual ~MeshHeap();

		void SetTransformDescStartIndex(unsigned int startIndex = 0);

		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<K3D::TextureObject>>& GetTexturesRef();

		std::weak_ptr<K3D::TextureObject> GetTexturteRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<K3D::TextureObject> textureRef);

		void Discard();

	protected:

	private:

	};
}

