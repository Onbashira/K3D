#pragma once
namespace K3D {

	class GameObject;

	class MaterialBase;

	class DescriptorHeap;

	class TargetState;

	class PipelineState;

	class PipelineResourceTableState;

	class GeometryState;

	//�P��`����T�|�[�g����x�[�X�R���|�l���g

	class GraphicsComponent
	{
	public:

	private:

	public:

		GraphicsComponent() {};

		virtual ~GraphicsComponent() {};

		virtual void Draw(TargetState* targetState, GeometryState* geometryState, PipelineState* pipelineStae, PipelineResourceTableState* prTbl) = 0;

	private:

	};
}