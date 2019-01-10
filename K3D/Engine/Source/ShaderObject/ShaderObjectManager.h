#pragma once
#include <d3d12.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace K3D {

	class PipelineStateObject;
	class RootSignature;

	class ShaderObjectManager
	{
	private:
		std::map <std::string, std::shared_ptr<K3D::PipelineStateObject>> _psolibrary;
		std::map <std::string, std::shared_ptr<K3D::RootSignature>> _rootSignaturelibrary;
	public:
	private:
	public:

		ShaderObjectManager();
		
		~ShaderObjectManager();

		//描画パイプラインの作成
		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob* signature = nullptr);
		
		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC cps, ID3DBlob* signature = nullptr);

		//ルートシグネチャの作成
		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		//ルートシグネチャの作成
		HRESULT CreateRootSignature(std::string rsName,D3D12_ROOT_SIGNATURE_DESC * desc);

		//パイプラインステートの登録
		void SetPSO(std::string psoName, std::shared_ptr<K3D::PipelineStateObject> pso);
		
		//ロートシグネチャの登録
		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		std::weak_ptr<K3D::PipelineStateObject> GetPSO(std::string psoName);
		
		std::weak_ptr<K3D::RootSignature> GetRootSignature(std::string rsName);

		void ErasePSO(std::string psoName);
		
		void EraseRootSignature(std::string psoName);

		void Discard();

	};
}