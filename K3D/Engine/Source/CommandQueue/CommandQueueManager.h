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
		//!�쐬���ꂽ�L���[��ێ�
		UnorderedManagerComponent<CommandQueue> _queues;
	
	public:

		CommandQueueManager();

		~CommandQueueManager();

		//�v�f�̍쐬
		HRESULT Create(std::string name, D3D12_COMMAND_QUEUE_DESC& desc);

		//�v�f�̎擾
		std::weak_ptr<CommandQueue> Get(std::string name);

		//�v�f�̍폜
		void Erase(std::string name);

		//�}�b�v�̖����I�N���A
		void DiscardMap();

		//���\�[�X�����[�h�ς݂��ۂ�
		bool IsLooded(std::string name);

		//�}�b�v�̔j��
		void Discard();

	private:

		void operator =(const CommandQueueManager&) {};

		CommandQueueManager(const CommandQueueManager&) {};

		void operator=(CommandQueueManager&&) {};

		CommandQueueManager(CommandQueueManager&&) {};

	};
}