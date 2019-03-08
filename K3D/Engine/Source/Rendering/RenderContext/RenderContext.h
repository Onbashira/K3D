#pragma once
namespace K3D {

	class CommandList;
	class CommandAllocator;
	class Fence;
	class CommandQueue;
	class D3D12Device;

	// 各シーン毎のレンダーコンテキスト
	class RenderContext
	{
	public:
		enum class RC_COMMAND_LIST_TYPE
		{
			BEGIN = 0,
			END = 1
		};
	private:

		int _frameNum;

		int _currentIndex;

		int _node;

		std::vector<std::array<std::shared_ptr<CommandList>, 2>> _cmdLists;

		std::vector<std::shared_ptr<CommandAllocator>> _cmdAllocators;

		std::mutex _allocatorMutex;

		std::vector<Fence> _fences;

		std::shared_ptr<CommandQueue> _queueRef;

		std::vector<std::shared_ptr<CommandList>> _listsVector;

	public:

		RenderContext();

		virtual ~RenderContext();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device,int frameNum, int nodeMask,std::shared_ptr<CommandQueue>& queue);

		HRESULT CreateCommandList(std::shared_ptr<D3D12Device>& device,D3D12_COMMAND_LIST_TYPE& type, std::shared_ptr<CommandList>& commandList);

		int GetCurrentIndex();

		int IncrementCount();

		std::weak_ptr<K3D::CommandList> GetCurrentList(RC_COMMAND_LIST_TYPE& listType);
		
		std::weak_ptr<K3D::CommandAllocator> GetCurrentCmdAllocator();

		K3D::Fence& GetCurrentFence();

		std::weak_ptr<CommandQueue> GetCommandQueue();

		void ExecuteCommandLists(std::shared_ptr<CommandQueue>& commandQueue);

		void ResetAllocators();

		void ResetCurrentCommandList();

		void ResetCommandList(std::shared_ptr<CommandList>& list);

		void Reset();

		void Discard();

	private:


	};

}