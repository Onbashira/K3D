#include "stdafx.h"
#include "InternalCommandAllocator.h"


K3D::InternalCommandAllocator::InternalCommandAllocator()
{
}


K3D::InternalCommandAllocator::~InternalCommandAllocator()
{
}

void K3D::InternalCommandAllocator::Reset()
{
	_allocatedMemory.clear();
}
