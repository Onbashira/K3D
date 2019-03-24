#include "stdafx.h"
#include "PrimitiveRenderer.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"

K3D::PrimitiveRenderer::PrimitiveRenderer()
{
}


K3D::PrimitiveRenderer::~PrimitiveRenderer()
{
}

void K3D::PrimitiveRenderer::Draw(std::shared_ptr<CommandList>& cmdList, GameObject * ojcet, GeometryState * geometryState, ModelMesh * mesh)
{

	auto& descriptorsItr = mesh->meshHeap->GetDescriptors().begin();
	//Camera View Set
	cmdList->SetGraphicsRootDescriptorTable(0, descriptorsItr->lock()->gpuAddress);
	//transform View Set
	++descriptorsItr;
	cmdList->SetGraphicsRootDescriptorTable(1, descriptorsItr->lock()->gpuAddress);

	//IA Stage
	auto gState = mesh->GetGeometryState();
	cmdList->IASetIndexBuffer(&gState.GetIndexBufferView());
	cmdList->IASetVertexBuffers(0,1,&gState.GetVertexBufferView());
	cmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->DrawIndexedInstanced(mesh->GetIndexListCount(), 1, 0, 0,0);

}

void K3D::PrimitiveRenderer::SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::PipelineStateObject> pipelineState)
{
	
}

void K3D::PrimitiveRenderer::SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::RootSignature> rootSignature)
{

}
