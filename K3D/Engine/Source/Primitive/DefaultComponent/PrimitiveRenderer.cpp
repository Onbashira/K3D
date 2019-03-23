#include "stdafx.h"
#include "PrimitiveRenderer.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Mesh/ModelMesh.h"

K3D::PrimitiveRenderer::PrimitiveRenderer()
{
}


K3D::PrimitiveRenderer::~PrimitiveRenderer()
{
}

void K3D::PrimitiveRenderer::Draw(std::shared_ptr<CommandList>& cmdList, GameObject * ojcet, GeometryState * geometryState, ModelMesh * mesh)
{
}

void K3D::PrimitiveRenderer::SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::PipelineStateObject> pipelineState)
{
}

void K3D::PrimitiveRenderer::SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::RootSignature> rootSignature)
{
}
