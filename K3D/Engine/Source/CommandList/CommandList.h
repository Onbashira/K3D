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
		//�O���t�B�N�X�R�}���h���X�g
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2>	_commandList;
		//�R�}���h���X�g
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		_commandAllocator;
		//�R�}���h���X�g�̖��O
		std::string											_commandListName;
		//�R�}���h�A���P�[�^�̖��O
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