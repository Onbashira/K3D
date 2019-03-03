#pragma once
namespace K3D {

	class CommandList;
	class CommandAllocator;
	class Fence;
	class CommandQueue;

	// 各シーン毎のレンダーコンテキスト

	class RenderContext
	{
	public:
		enum class RenderCommandList
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

	public:

		RenderContext();

		virtual ~RenderContext();

		HRESULT Create(int frameNum, int nodeMask,std::shared_ptr<CommandQueue>& queue);

		int GetCurrentIndex();

		int IncrementCount();

		std::weak_ptr<K3D::CommandList> GetCurrentList(RenderCommandList& listType);
		
		std::weak_ptr<K3D::CommandList> GetCurrentCmdAllocator();

		K3D::Fence& GetCurrentFence();

		void ResetAllocators();

		void ResetCurrentCommandList();

		void ResetCommandList(std::shared_ptr<CommandList>& list);

		void Reset();

		void Discard();

	private:


	};

}