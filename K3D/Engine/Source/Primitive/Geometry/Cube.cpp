#include "Cube.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveInputComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitivePhysicsComponent.h"
#include "Engine/Source/Primitive/DefaultComponent/PrimitiveRenderer.h"
#include "Engine/Source/Component/ComponentsHeader.h"

K3D::Cube::Cube(std::shared_ptr<GameHeap>& heap) :
	PrimitiveObject(new PrimitiveRenderer(), new PrimitiveInputComponent(), new PrimitivePhysicsComponent() ,heap)
{

}


K3D::Cube::~Cube()
{
}

void K3D::Cube::Update()
{
}

void K3D::Cube::Draw()
{
}

void K3D::Cube::MeshCreate()
{
}

void K3D::Cube::ComponentInitialize()
{
}
