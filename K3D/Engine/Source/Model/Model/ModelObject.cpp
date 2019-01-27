#include "ModelObject.h"


K3D::ModelObject::ModelObject()
{

}


K3D::ModelObject::~ModelObject()
{
	DiscardModelObject();
}

void K3D::ModelObject::DiscardModelObject()
{
	this->DiscardMeshHeap();
	this->DiscardMeshBuffer();
}