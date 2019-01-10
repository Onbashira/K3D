#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
namespace K3D {

	class CommandList;
	class Resource;
	class DescriptorHeap;

	class TemporarilyBuffer
	{
	public:

	private:
		Microsoft::WRL::ComPtr<ID3D12DeviceChild> _pObj;
		int _lifeTime;
	public:

		TemporarilyBuffer();

		TemporarilyBuffer(Microsoft::WRL::ComPtr<ID3D12DeviceChild> pObj, int lifeTime);

		TemporarilyBuffer(CommandList* pObj, int lifeTime);

		TemporarilyBuffer(Resource* pObj, int lifeTime);

		TemporarilyBuffer(DescriptorHeap* pObj, int lifeTime);

		~TemporarilyBuffer();

		void TimeDeclement();

		void Discard();

	private:
	};

}