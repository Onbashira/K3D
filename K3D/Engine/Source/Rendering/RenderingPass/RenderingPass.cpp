#include "stdafx.h"
#include "RenderingPass.h"


K3D::RenderingPass::RenderingPass(std::shared_ptr<RenderingPassHolder> parentHolder) :
	_parentHolder(parentHolder)
{

}

K3D::RenderingPass::~RenderingPass()
{
}

