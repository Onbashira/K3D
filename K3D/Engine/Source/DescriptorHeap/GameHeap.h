#pragma once
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"
namespace K3D {

	class Resource;
	class RenderingDevice;
	class D3D12Device;
	class CommandList;

	class GameHeap
	{
	public:

		enum class ViewType {
			CBV, SRV, UAV, RTV, DSV, SAMP, EMPTY
		};

		enum class HeapType {
			CPU, RTV, DSV, SAMP,EMPTY
		};

		struct Descriptor {

			unsigned int offset;

			K3D::GameHeap::ViewType viewType;

			K3D::GameHeap::HeapType heapType;

			D3D12_GPU_DESCRIPTOR_HANDLE gpuAddress;

			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;

			Descriptor() :
				offset(), viewType(K3D::GameHeap::ViewType::EMPTY), gpuAddress(D3D12_GPU_DESCRIPTOR_HANDLE()), cpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE()) {};

			Descriptor(unsigned int offset, K3D::GameHeap::ViewType& viewType, D3D12_GPU_DESCRIPTOR_HANDLE& gpuAddress, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle) :
				offset(), viewType(viewType), gpuAddress(gpuAddress), cpuHandle(cpuHandle) {};



			Descriptor(Descriptor&& other) {
				*this = std::move(other);

			};


			Descriptor& operator= (Descriptor&& other) {
				*this = other;

				other.offset = 0;
				other.viewType = K3D::GameHeap::ViewType::EMPTY;
				other.heapType = K3D::GameHeap::HeapType::EMPTY ;
				other.gpuAddress = D3D12_GPU_DESCRIPTOR_HANDLE();
				other.cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE();
				
				return *this;
			};

		private:

			Descriptor(const Descriptor& other) {
				*this = other;
			};

			Descriptor& operator= (const Descriptor& other) {
				offset = other.offset;
				viewType = other.viewType;
				heapType = other.heapType;
				gpuAddress = other.gpuAddress;
				cpuHandle = other.cpuHandle;
				return *this;
			};
		};

		using DescHeapMap = std::map<K3D::GameHeap::HeapType, std::shared_ptr<DescriptorHeap>>;

		using ViewMap = std::map<K3D::GameHeap::HeapType, std::vector<ViewType>>;

	private:

		std::shared_ptr<D3D12Device> _device;

		DescHeapMap _heaps;

		ViewMap _allocatedViewMap;

		unsigned int _cpuOffset;

		unsigned int _rtvOffset;

		unsigned int _dsvOffset;

		unsigned int _samplerOffset;

		std::mutex _cpuMutex;

		std::mutex _rtvMutex;

		std::mutex _dsvMutex;

		std::mutex _samplerMutex;


	public:

		GameHeap(unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		GameHeap(std::shared_ptr<D3D12Device>& device, unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		~GameHeap();

		HRESULT ReInitialize(unsigned int maxCPUHeapSize, unsigned int maxRTHeapSize, unsigned int maxDSHeapSize, unsigned int maxSampHeapSize);

		HRESULT ReCreateHeap(const HeapType& heapType, unsigned int maxHeapSize);

		K3D::GameHeap::Descriptor GetDescriptorHandle(const K3D::GameHeap::HeapType& heapType, unsigned int handleOffset);

		K3D::GameHeap::Descriptor CreateView(const K3D::GameHeap::HeapType& heapType, const K3D::GameHeap::ViewType& viewType, void* viewDesc, unsigned int handleOffset, Resource* resource, Resource* counterResource = nullptr);

		K3D::GameHeap::Descriptor CreateCBView(D3D12_CONSTANT_BUFFER_VIEW_DESC& desc);

		K3D::GameHeap::Descriptor CreateSRView(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, Resource* resource);

		K3D::GameHeap::Descriptor CreateUAView(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, Resource* resource, Resource* counterResource = nullptr);
		
		K3D::GameHeap::Descriptor CreateRTView(D3D12_RENDER_TARGET_VIEW_DESC& desc, Resource* resource);

		K3D::GameHeap::Descriptor CreateDSView(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, Resource* resource);

		K3D::GameHeap::Descriptor CreateSampView(D3D12_SAMPLER_DESC& desc);

		void Discard();

	private:



	};

}