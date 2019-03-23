#pragma once
#include "Engine/Source/Component/GameObject/GameObject.h"

namespace K3D {

	class ModelMesh;
	class GameHeap;

	class PrimitiveObject :public GameObject
	{
	public:
	private:

		std::unique_ptr<ModelMesh> _modelMesh;

	public:

		PrimitiveObject(GraphicsComponent* graphicsComponent, InputComponent* inputComponent, PhysicsComponent* physicsComponent, std::shared_ptr<GameHeap>& _gameHeap);

		virtual ~PrimitiveObject();
	private:

		PrimitiveObject();

	};

}