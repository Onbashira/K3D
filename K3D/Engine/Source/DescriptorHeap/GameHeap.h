#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
namespace K3D {
	class GameHeap
	{
	public:

	private:

		std::shared_ptr<DescriptorHeap> _cpuHeap;
		std::shared_ptr<DescriptorHeap> _samplerHeap;
		std::shared_ptr<DescriptorHeap> _rtvHeap;
		std::shared_ptr<DescriptorHeap> _dsvHeap;

	public:
		GameHeap(unsigned int maxDescriptorSize);
		~GameHeap();
	};

}