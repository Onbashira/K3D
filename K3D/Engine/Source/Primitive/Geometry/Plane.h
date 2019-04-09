#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"

namespace K3D {

	class GameHeap;

	class Plane : public PrimitiveObject
	{
	public:

		Plane(std::shared_ptr<GameHeap>& heap);

		virtual ~Plane();

		virtual void Update() override;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) override;

		void Initialize();

		void MeshCreate();

		void CreateDescriptors();

		void ComponentInitialize();

	};
}

