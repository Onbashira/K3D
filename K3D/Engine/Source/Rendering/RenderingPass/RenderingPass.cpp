#include "stdafx.h"
#include "RenderingPass.h"


K3D::RenderingPass::RenderingPass(std::shared_ptr<RenderingPassHolder> parentHolder) :
	_parentHolder(parentHolder)
{

}

K3D::RenderingPass::~RenderingPass()
{
	Discard();
}

void K3D::RenderingPass::Discard()
{
	if (_prePathRenderTarges.size() > 0) {
		_prePathRenderTarges.clear();
		_prePathRenderTarges.shrink_to_fit();
	}
}


