#include "stdafx.h"
#include "RenderingPathHolder.h"


K3D::RenderingPathHolder::RenderingPathHolder()
{
}


K3D::RenderingPathHolder::~RenderingPathHolder()
{
}

void K3D::RenderingPathHolder::RegistRenderingPass(std::shared_ptr<RenderingPass> pass)
{
	this->_rpList.push_back(pass);
}
