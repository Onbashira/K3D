#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Resource.h"

namespace K3D {
	class IndexBuffer : public Resource
	{
		D3D12_INDEX_BUFFER_VIEW _view;

		ULONG64 _size;

	public:

		IndexBuffer();

		~IndexBuffer();

		virtual HRESULT Initialize(ULONG64 size, DXGI_FORMAT format, const void * pIndices);

		virtual HRESULT Initialize(ULONG64 size, size_t elementSize, const void * pIndices);

		void Discard();

		D3D12_INDEX_BUFFER_VIEW GetView()const;

	private:


	};
}