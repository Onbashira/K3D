#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <vector>
namespace K3D {
	class TargetState
	{
	public:
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> _rtvCPUHandles;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> _dsvCPUHandle;
		std::vector<D3D12_VIEWPORT> _viewports;
	private:
	public:
		TargetState();
		~TargetState();
	private:
	};

}