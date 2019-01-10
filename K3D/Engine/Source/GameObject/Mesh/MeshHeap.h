#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include "Engine/Source/Resource/ConstantBuffer.h"
#include <memory>
#include <vector>

namespace K3D {
	class ShaderResource;
	class CommandList;

	//Meshをレンダリングするためのクラス。主にシェーダーに投げるためのヒープを管理する
	class MeshHeap
	{
	private:
	
		K3D::DescriptorHeap									_heap;					//デスクリプタヒープ　					(変形情報 + マテリアル数 + テクスチャ数)　カメラの情報は別で投げる
	
		unsigned int										_transformStartPoint;	//変形情報デスクリプタの開始位置		(maybe 0)
	
		unsigned int										_materialStartPoint;	//マテリアルデスクリプタの開始位置		(maybe 0+1 ~ x)
		
		unsigned int										_textureStartPoint;		//テクスチャデスクリプタの開始位置		(maybe x+1 ~ y)

		K3D::ConstantBuffer									_materialBuffer;		//マテリアル情報のバッファ 　　
	
		std::vector<std::weak_ptr<K3D::ShaderResource>>		_textureResource;		//テクスチャリソースへの参照	
	public:

	protected:

	private:

	public:

		void BindingDescriptorHeaps(std::weak_ptr<K3D::CommandList> list);

		void BindingDescriptorHeaps(K3D::CommandList& lsit);

		void SetTransformDescStartIndex(unsigned int startIndex = 0);
		
		void SetMaterialDescStartIndex(unsigned int startIndex = 1);

		void SetTextureDescStartIndex(unsigned int startIndex = 2);

		unsigned int GetTransformDescStartIndex();

		unsigned int GetTextureDescStartIndex();

		unsigned int GetMaterialDescStartIndex();

		K3D::DescriptorHeap& GetHeap();

		ConstantBuffer& GetMaterialBufffer();

		std::vector<std::weak_ptr<K3D::ShaderResource>>& GetTexturesRef();

		std::weak_ptr<K3D::ShaderResource> GetTexturteRef(unsigned int textureIndex);

		MeshHeap& AddTextureRef(std::weak_ptr<K3D::ShaderResource> textureRef);
		
		void Discard();
		
		MeshHeap();
		virtual ~MeshHeap();
	};
}

