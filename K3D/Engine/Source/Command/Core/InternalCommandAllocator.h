#pragma once
#include "Engine/Source/Command/Core/InternalCommand.h"

namespace K3D {

	//中間描画命令リスト管理用クラス
	class InternalCommandAllocator
	{
		friend class CommandManager;

	public:

	private:

		std::mutex _allocatorMutex;


		std::list<InternalCommand> _allocatedMemory;

	public:

		InternalCommandAllocator();

		~InternalCommandAllocator();

		void Reset();

	private:
		

	};

}