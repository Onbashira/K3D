#include "stdafx.h"
#include "SpriteRenderer.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"

K3D::SpriteRenderer::SpriteRenderer()
{
}


K3D::SpriteRenderer::~SpriteRenderer()
{
}

void K3D::SpriteRenderer::Draw(std::shared_ptr<CommandList>& cmdList, GameObject * ojcet, GeometryState * geometryState, ModelMesh * mesh)
{

	auto& descriptorsItr = mesh->meshHeap->GetDescriptors().begin();
	//Material View Set
	cmdList->SetGraphicsRootDescriptorTable(0, descriptorsItr->lock()->gpuAddress);
	//IA Stage
	static GeometryState gState ;
	gState = mesh->GetGeometryState();
	cmdList->IASetIndexBuffer(&gState.GetIndexBufferView());
	cmdList->IASetVertexBuffers(0,1,&gState.GetVertexBufferView());
	cmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->DrawIndexedInstanced(static_cast<unsigned int >(mesh->GetIndexListCount()), 1, 0, 0,0);

}

void K3D::SpriteRenderer::SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::PipelineStateObject> pipelineState)
{
	cmdList->SetPipelineState(pipelineState.lock());
}

void K3D::SpriteRenderer::SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<K3D::RootSignature> rootSignature)
{
	cmdList->SetGraphicsRootSignature(rootSignature.lock());

}
