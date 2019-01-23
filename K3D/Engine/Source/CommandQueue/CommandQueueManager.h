#pragma once

#include "Engine/Source/CommandQueue/CommandQueue.h"
#include "Engine/Source/Utility/ManagerComponent.h"
#include "Engine/Source/Utility/NonCopyable.h"

namespace K3D {
	class CommandQueue;
	class CommandQueueManager : private NonCopyable
	{
		friend class GraphicsContextManager;

	public:
		
	private:
		//!作成されたキューを保持
		UnorderedManagerComponent<CommandQueue> _queues;
	
	public:

		CommandQueueManager();

		~CommandQueueManager();

		//要素の作成
		HRESULT Create(std::string name, D3D12_COMMAND_QUEUE_DESC& desc);

		//要素の取得
		std::weak_ptr<CommandQueue> Get(std::string name);

		//要素の削除
		void Erase(std::string name);

		//マップの明示的クリア
		void DiscardMap();

		//リソースがロード済みか否か
		bool IsLooded(std::string name);

		//マップの破棄
		void Discard();

	private:

		void operator =(const CommandQueueManager&) {};

		CommandQueueManager(const CommandQueueManager&) {};

		void operator=(CommandQueueManager&&) {};

		CommandQueueManager(CommandQueueManager&&) {};

	};
}