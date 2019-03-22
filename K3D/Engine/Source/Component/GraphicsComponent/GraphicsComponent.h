#pragma once
namespace K3D {

	class GameObject;

	class MaterialBase;

	class DescriptorHeap;

	class TargetState;

	class PipelineState;

	class PipelineResourceTableState;

	class GeometryState;

	class PipelineStateObject;

	class RootSignature;

	class CommandList;

	//単一描画をサポートするベースコンポネント

	class GraphicsComponent
	{
	public:

	private:

	public:

		GraphicsComponent() {};

		virtual ~GraphicsComponent() {};

		virtual void Draw(TargetState* targetState, GeometryState* geometryState, PipelineState* pipelineStae, PipelineResourceTableState* prTbl) = 0;

		virtual void SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState) = 0;

		virtual void SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature) = 0;

		virtual void SetCommandList(std::shared_ptr<K3D::CommandList> CommandList) = 0;

		virtual void BindingShaderObject() = 0;

		virtual void BindingShaderObjectToBundle() = 0;

		virtual void Draw() = 0;

	private:

		virtual void RegisterToBundle() = 0;

	};
}