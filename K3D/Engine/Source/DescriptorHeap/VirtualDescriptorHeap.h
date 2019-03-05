#pragma once

namespace K3D {

	enum class VDecsriptorHeapType
	{
		CBV_SRV_UAV,
		SAMPLER,
	};

	struct VDescriptorHeapDesc {

		unsigned int gameHeapAddress;
		VDecsriptorHeapType type;
	};

	class VirtualDescriptorHeap
	{
	public:

	private:
		

	public:
		VirtualDescriptorHeap();
		virtual ~VirtualDescriptorHeap();
	};
}