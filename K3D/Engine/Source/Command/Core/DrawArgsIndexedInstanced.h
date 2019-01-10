#pragma once
namespace K3D {
	struct DrawArgsIndexedInstanced
	{
		unsigned int IndexCountPerInstanced;
		unsigned int InstanceCount;
		unsigned int StartIndexLocation;
		unsigned int BaseVertexLocation;
		unsigned int StartInstanceLocation;
	};
}