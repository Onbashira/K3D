#pragma once
#include "Engine/Source/Command/State/GeometryState.h"

namespace K3D {

	class IndexBuffer;
	class VertexBuffer;

	//��{���b�V���N���X
	class BasicMesh
	{
	public:

	protected:

		//!Mesh�̌ŗLID
		unsigned int _meshID;
		//!���b�V�����\������W�I���g���X�e�[�g
		GeometryState _geometry;
		//!�W�I���g�����\�[�X
		std::unique_ptr<VertexBuffer> _meshVBO;
		//!�W�I���g�����\�[�X
		std::unique_ptr<IndexBuffer> _meshIBO;

	private:

	public:

		BasicMesh() {};

		virtual ~BasicMesh() {};

		void CreateVBO(ULONG64 size, UINT stride, const void* pVertices);

		void CreateIBO(ULONG64 size, size_t elementSize, const void * pIndices);

		void SetMeshID(Uint32 id);

		std::unique_ptr<VertexBuffer>& GetVBO();

		std::unique_ptr<IndexBuffer>& GetIBO();

		GeometryState& GetGeometryState();

		unsigned int GetID();

		void Discard();

	private:
	
	};
}