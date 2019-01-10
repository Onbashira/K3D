#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/Async/Fence.h"
#include <string>
namespace K3D {

	class CommandQueue
	{


	public:
	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>	_commandQueue;

		K3D::Fence	_fence;

		D3D12_COMMAND_QUEUE_DESC _desc;

		std::string _name;

	public:

		CommandQueue();

		~CommandQueue();

		HRESULT												Create(D3D12_COMMAND_QUEUE_DESC& desc);

		Microsoft::WRL::ComPtr<ID3D12CommandQueue>			GetQueue()const;

		void												Wait(Fence* fence = nullptr);

		UINT64												GetTimestampFrequency();

		D3D12_COMMAND_QUEUE_DESC							GetDesc();

		void SetName(std::string name);

		void Discard();

	private:
	};
}

