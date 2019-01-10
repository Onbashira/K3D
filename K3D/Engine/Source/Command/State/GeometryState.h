#pragma once
#include "Engine/Source/Utility/D3D12Common.h"

namespace K3D {
	class GeometryState
	{
	public:

	private:

		D3D12_VERTEX_BUFFER_VIEW _vertexBuffer;

		D3D12_INDEX_BUFFER_VIEW  _indexBuffer;

	public:

		GeometryState();

		~GeometryState();

		D3D12_VERTEX_BUFFER_VIEW& GetVertexBuffer();

		D3D12_INDEX_BUFFER_VIEW& GetIndexBuffef();

		D3D12_VERTEX_BUFFER_VIEW SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW& vertexBuffer);

		D3D12_INDEX_BUFFER_VIEW SetIndexBuffef(D3D12_INDEX_BUFFER_VIEW& indexBuffer);



	private:

	};
}