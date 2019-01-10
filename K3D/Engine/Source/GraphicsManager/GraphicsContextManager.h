#pragma once
#include"Engine/Source/Utility/D3D12Common.h"
#include"Engine/Source/Utility//NonCopyable.h"
#include"Engine/Source/CommandList/CommandListManager.h"
#include"Engine/Source/ShaderObject/ShaderObjectManager.h"
#include "Engine/Source/CommandQueue/CommandQueueManager.h"
#include <vector>



namespace K3D {
	class CommandQueue;
	class DepthStencil;
	class Fence;
	class PipelineStateObject;


	class GraphicsContextManager : private NonCopyable
	{
	private:

		CommandListManager			_commandListLibrary;

		ShaderObjectManager			_shaderObjectLibrary;

		CommandQueueManager         _queueManager;

	public:
	private:

		GraphicsContextManager();

	public:

		~GraphicsContextManager();

		void    CloseAllCommandLists();

		void	CloseCommandList(std::string commandListName);

		void	ResetAllCommandList();

		void    ResetCommandList(std::string commandListName);

		HRESULT ExcutionCommandListAsync(CommandQueue* queue, std::vector<std::string> commandListNames, Fence* fence = nullptr);

		HRESULT ExcutionAllCommandLists(CommandQueue* queue, Fence* fence = nullptr);

		HRESULT CreateCommandList(std::string commandListName, unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType);

		HRESULT CreatePSO(std::string psoName, D3D12_GRAPHICS_PIPELINE_STATE_DESC gps, ID3DBlob* rootSignature = nullptr);

		HRESULT CreatePSO(std::string psoName, D3D12_COMPUTE_PIPELINE_STATE_DESC gps, ID3DBlob * rootSignature = nullptr);

		HRESULT CreateCommandQueue(std::string queueName, D3D12_COMMAND_QUEUE_DESC& desc);

		HRESULT CreateRootSignature(std::string rsName, ID3DBlob* signature);

		HRESULT CreateRootSignature(std::string rsName, D3D12_ROOT_SIGNATURE_DESC* signature);

		void SetCommandList(std::string commandListName, std::shared_ptr<CommandList> commandList);

		void SetPSO(std::string psoName, std::shared_ptr<PipelineStateObject> pso);

		void SetRootSignature(std::string rsName, std::shared_ptr<RootSignature> rootSignature);

		std::shared_ptr<CommandList> GetCommandList(std::string commandListName);

		std::shared_ptr<PipelineStateObject> GetPSO(std::string psoName);

		std::shared_ptr<CommandQueue> GetCommandQueue(std::string queueName);

		std::shared_ptr<RootSignature> GetRootSignature(std::string rsName);

		void EraseCommandList(std::string commandListName);

		void ErasePSO(std::string psoName);

		void EraseRootSignature(std::string psoName);

		void EraseCommandQueue(std::string queueName);

		void Discard();

		static GraphicsContextManager& GetInstance();
	};
};