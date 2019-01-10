#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <string>

namespace K3D {

	class RootSignature
	{
	public:

	private:

		Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;

		std::string									_name;

	public:

		RootSignature();

		~RootSignature();

		HRESULT CreateFromShader(ID3DBlob* shader);

		HRESULT CreateFromDesc(D3D12_ROOT_SIGNATURE_DESC* desc);

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature();

		void SetName(std::string name);

		void Discard();

	private:

	};
}