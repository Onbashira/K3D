#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <map>
#include <string>


namespace K3D {

	class CommandQueue;
	class Fence;
	class PipelineStateObject;
	class CommandListManager;

	class CommandList
	{
		friend class CommandListManager;

	public:

	private:
		//グラフィクスコマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>	_commandList;
		//コマンドリスト
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		_commandAllocator;
		//コマンドリストの名前
		std::string											_commandListName;
		//コマンドアロケータの名前
		std::string											_commandAllocatorName;
		//
		D3D12_COMMAND_LIST_TYPE								_listType;


	public:

		CommandList();

		~CommandList();

		HRESULT Create(unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		HRESULT SetResourceBarrie(ID3D12Resource * resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>	GetCommandList()const;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		GetAllocator()const;

		HRESULT						ResetCommandList(ID3D12PipelineState* pInitialState = nullptr);

		HRESULT						ResetAllocator();

		HRESULT						CloseCommandList();

		void						Discard();

		void						SetName(std::string objectName);

		void						SetCommandListName(std::string name);

		void						SetCommandAllocatorName(std::string name);
	private:

	};

};