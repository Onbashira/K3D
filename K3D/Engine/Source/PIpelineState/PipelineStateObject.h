#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include <memory>
#include <string>

namespace K3D {

	class RootSignature;
	class PipelineStateObject
	{	
	public:

	protected:

	private:

		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;

		std::weak_ptr<RootSignature>				_rootSignature;

		std::string									_name;

	public:		

		PipelineStateObject();

		virtual ~PipelineStateObject();

		virtual HRESULT								Create(std::string name ,D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, ID3DBlob* shader);

		virtual HRESULT								Create(std::string name ,D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, ID3DBlob* shader);

		virtual HRESULT								Create(std::string name, D3D12_GRAPHICS_PIPELINE_STATE_DESC & gpsDesc, std::weak_ptr<RootSignature> rootSignature);

		virtual HRESULT								Create(std::string name, D3D12_COMPUTE_PIPELINE_STATE_DESC &  cpsDesc, std::weak_ptr<RootSignature> rootSignature);

		void										SetName(std::string name);

		void										BindingRootSignature(std::weak_ptr<RootSignature> rootSignature);

		std::weak_ptr<RootSignature>				GetRootSignature();

		Microsoft::WRL::ComPtr<ID3D12PipelineState>	GetPSO();

		virtual void Discard();

	private:

	};
}
