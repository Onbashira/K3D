#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
#include <memory>

namespace K3D {

	class PipelineStateObject;
	class CommandList;
	class RootSignature;

	//�`��\�R���|�l���g
	class DrawableComponent{

	private:

	protected:

	public:

	private:

	protected:

		//�o���h�����X�g�Ɍp������L�̃h���[�R�[����ςލ�Ƃ�����
		virtual void RegisterToBundle() = 0;

	public:

		DrawableComponent() {};

		virtual ~DrawableComponent() {};

		virtual void SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState) = 0;

		virtual void SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature) = 0;

		virtual void SetCommandList(std::shared_ptr<K3D::CommandList> masterCommandList) = 0;

		virtual void BindingShaderObject() = 0;

		virtual void BindingShaderObjectToBundle() = 0;

		virtual void Draw() = 0;
	};
}

