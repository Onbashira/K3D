#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"
namespace K3D {
	class Cube : public PrimitiveObject
	{
	public:

	private:
		
	public:

		Cube(std::shared_ptr<GameHeap>& heap);

		virtual ~Cube();

		virtual void Update() override;

		virtual void Draw() override;
	
	private:

		void MeshCreate();

		void ComponentInitialize();
	};
}