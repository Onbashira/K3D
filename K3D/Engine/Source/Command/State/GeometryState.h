#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace K3D {
	class GeometryState
	{
	public:

	private:

		//!頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW _vertexBuffer;
		//!インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW  _indexBuffer;

	public:

		GeometryState();

		~GeometryState();

		/**
		* @fn
		* @brief 頂点バッファの取得
		* @return  頂点バッファへの参照
		*/
		D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer();

		/**
		* @fn
		* @brief インデックスバッファの取得
		* @return  インデックスバッファへの参照
		*/
		D3D12_INDEX_BUFFER_VIEW& GetIndexBuffef();

		/**
		* @fn
		* @brief 頂点バッファのセット
		* @oaram[in] vertexBuffer　VBO
		*/
		void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW& vertexBuffer);

		/**
		* @fn
		* @brief インデックスバッファのセット
		* @oaram[in] vertexBuffer　VBO
		*/
		void SetIndexBuffef(D3D12_INDEX_BUFFER_VIEW& indexBuffer);

	private:

	};
}