#include "stdafx.h"
#include "ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"

K3D::ModelMesh::ModelMesh() :
	meshBuffer(new MeshBuffer()), meshHeap(new MeshHeap)
{

}

K3D::ModelMesh::~ModelMesh()
{
}
