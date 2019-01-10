#pragma once
#include "Engine/Source/State/GeometryState.h"


namespace K3D {
	
	class IndexBuffer;
	class VertexBuffer;

	class Mesh
	{
	public:

	private:

		unsigned int _meshID;

		GeometryState _geometry;

	public:
		
		Mesh() {};
		
		virtual ~Mesh() {};

	private:
	};
}