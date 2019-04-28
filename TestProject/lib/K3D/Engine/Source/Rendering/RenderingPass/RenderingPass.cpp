#include "stdafx.h"
#include "RenderingPass.h"
#include "Engine/Source/CommandList/CommandList.h"


K3D::RenderingPass::RenderingPass(std::shared_ptr<CommandList> cmdList, std::shared_ptr<GameHeap>& _gameHeap)
{
}

K3D::RenderingPass::~RenderingPass()
{
}

std::shared_ptr<K3D::CommandList> K3D::RenderingPass::GetCommandList()
{
	return _cmdList;
}
