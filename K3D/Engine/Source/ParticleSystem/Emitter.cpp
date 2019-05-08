#include "stdafx.h"
#include "Emitter.h"


K3D::Emitter::Emitter()
{
}


K3D::Emitter::~Emitter()
{
}

void K3D::Emitter::UpdateItems()
{
}

void K3D::Emitter::SetCustomUpdater(std::function<void(void)> function)
{
	_customUpdater = function;
}
