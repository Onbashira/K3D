#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/LifetimedMemory/LifetimedMemory.h"
#include <tuple>
#include <string>


namespace K3D {

	class CommandList;

	class Resource : public enable_lifetimed_shared_from_this<Resource>
	{

	private:

	protected:
		
		D3D12_RESOURCE_STATES					_currentResourceState;
		
		UCHAR*									_pDst;
		
		D3D12_CLEAR_VALUE						_clearValue;
	
		unsigned int							_shaderRegisterNumber;
	
		Microsoft::WRL::ComPtr<ID3D12Resource>	_resource;
	
		std::string								_name;

	private:

	public:
		
		Resource();
		
		Resource(const Resource& other);
		
		Resource(Resource&& value);
		
		Resource& operator= (const Resource& value);
		
		Resource& operator= (Resource&& value);
		
		virtual ~Resource();

		static LifetimedShared_Ptr<Resource>								Create();
		
		virtual HRESULT														Create(const D3D12_HEAP_PROPERTIES& heapProps,const D3D12_HEAP_FLAGS& flags,const D3D12_RESOURCE_DESC& resourceDesc,const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);
		
		virtual HRESULT														Map(UINT subResource, D3D12_RANGE* readRange);
		
		virtual void														Unmap(UINT subResource, D3D12_RANGE* writtenRange);
		
		UCHAR*																GetMappedPointer();
	
		void																Discard();
		
		virtual void														Update(const void* pSrc, ULONG64 size, const UINT dstOffset);
		
		virtual void														Read(void* pDstBuffer,ULONG64 readSize, const unsigned int dstOffsest = 0);
		
		void																Update(const void* pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE* readRange = nullptr, D3D12_RANGE* writtenRange = nullptr);
		
		void																Alignment256ByteUpdate(const void* pSrc, unsigned int elementSize, unsigned int elementCount = 1);
		
		Microsoft::WRL::ComPtr<ID3D12Resource>&								GetResource();
				
		const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS>			GetHeapPropaties();
		
		const D3D12_RESOURCE_DESC*											GetResourceDesc();
		
		const D3D12_RESOURCE_STATES&										GetResourceState();
		
		void																SetResourceState(D3D12_RESOURCE_STATES state);
		
		HRESULT																ResourceTransition(CommandList* list, D3D12_RESOURCE_STATES nextState);
		
		HRESULT																ResourceTransition(std::weak_ptr<CommandList> list, D3D12_RESOURCE_STATES nextState);
		
		D3D12_CLEAR_VALUE													GetClearValue();
		
		void																SetName(std::string name);
		
		void																RegisterShaderSlot(unsigned int number);

	private:

	private:	

	};
}