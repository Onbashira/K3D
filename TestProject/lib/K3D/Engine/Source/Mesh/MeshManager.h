#pragma once
#include "Engine/Source/Utility/ManagerComponent.h"
namespace K3D {
	class MeshData;
	class MeshManager
	{
	public:
	private:
		UnorderedManagerComponent<int> _meshMap;
	public:
		MeshManager();
		~MeshManager();
	};
}