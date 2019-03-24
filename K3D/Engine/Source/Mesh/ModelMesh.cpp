#include "stdafx.h"
#include "ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"
#include "Engine/Source/Mesh/BasicMesh.h"

K3D::GeometryState K3D::ModelMesh::GetGeometryState()
{
	return  meshBuffer->_mesh.GetGeometryState();
}

unsigned int K3D::ModelMesh::GetIndexListCount()
{
	return meshBuffer->_mesh.GetIBO()->GetElementSize();
}

K3D::ModelMesh::ModelMesh() :
	meshBuffer(new MeshBuffer()), meshHeap(new MeshHeap)
{

}

K3D::ModelMesh::~ModelMesh()
{
}
