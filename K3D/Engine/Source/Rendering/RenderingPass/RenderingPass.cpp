#include "stdafx.h"
#include "RenderingPass.h"


K3D::RenderingPass::RenderingPass()
{
}


K3D::RenderingPass::~RenderingPass()
{
}

std::vector<std::shared_ptr<K3D::Resource>>& K3D::RenderingPass::GetPrePathRenderTargets()
{
	 return _prePathRenderTarges;
}
