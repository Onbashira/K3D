#include "stdafx.h"
#include "PrimitiveObject.h"
#include "Engine/Source/Mesh/ModelMesh.h"



K3D::PrimitiveObject::PrimitiveObject(GraphicsComponent * graphicsComponent, InputComponent * inputComponent, PhysicsComponent * physicsComponent, std::shared_ptr<GameHeap>& gameHeap) :
	GameObject(graphicsComponent , inputComponent, physicsComponent,gameHeap), _modelMesh(new ModelMesh)
{
}

K3D::PrimitiveObject::~PrimitiveObject()
{
}
