#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <memory>

namespace K3D {

	class PipelineStateObject;
	class CommandList;
	class RootSignature;

	//描画可能コンポネント
	class DrawableComponent
	{
	private:

	protected:

		unsigned int _drawCommandHandle;

		std::weak_ptr<K3D::PipelineStateObject>	_pipelineState;

		std::weak_ptr<K3D::RootSignature>		_rootSignature;

		std::weak_ptr<K3D::CommandList>			_commandList;

		K3D::CommandList						_bundleList;

	public:
	private:
	protected:
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle() = 0;
	public:

		DrawableComponent();

		virtual ~DrawableComponent();

		void SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState);

		void SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature);

		void SetCommandList(std::shared_ptr<K3D::CommandList> masterCommandList);

		void BindingShaderObject();

		void BindingShaderObjectToBundle();

		//描画時呼び出し関数の作成を強制
		virtual void Draw() = 0;
	};
}

