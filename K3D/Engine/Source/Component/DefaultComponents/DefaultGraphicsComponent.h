#pragma once
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
namespace K3D {
	class DefaultGraphicsComponent :
		public GraphicsComponent
	{
	public:
		DefaultGraphicsComponent();
		virtual ~DefaultGraphicsComponent();

		virtual void Draw(std::shared_ptr<CommandList>& cmdList, GameObject* object, GeometryState* geometryState, ModelMesh* mesh)override {};

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::PipelineStateObject> pipelineState)override {};

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::RootSignature> rootSignature)override {};
	};

}