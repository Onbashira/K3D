#pragma once
#include <string>
#include "Engine/Source/Utility/D3D12Common.h"

namespace K3D {

	class RootSignature;
	class CommandSignature
	{
	public:

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandSignature>	 _commandSignature;

		std::string										 _name;

	public:

		CommandSignature();

		~CommandSignature();

		//コマンドバッファの解釈を記述したDESCとコマンドシグネチャを適用するRootSignatureを引数に取る
		HRESULT	Create(const D3D12_COMMAND_SIGNATURE_DESC& desc, std::weak_ptr<K3D::RootSignature> rs);

		//コマンドバッファの解釈を記述したDESCとコマンドシグネチャを適用するRootSignatureを引数に取る
		HRESULT	Create(const D3D12_COMMAND_SIGNATURE_DESC& desc);

		Microsoft::WRL::ComPtr<ID3D12CommandSignature> GetSignature();

		void	SetName(std::string name);

		void	Discard();
	private:
	};
}
