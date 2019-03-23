#pragma once

#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"

namespace K3D {
	class PrimitiveRenderer : public GraphicsComponent
	{

	public:

	private:

	public:

		PrimitiveRenderer();

		virtual ~PrimitiveRenderer();

		virtual void Draw(std::shared_ptr<CommandList>& cmdList,GameObject* ojcet, GeometryState* geometryState,ModelMesh* mesh)override;

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::PipelineStateObject> pipelineState)override;

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::RootSignature> rootSignature)override;


	};

}