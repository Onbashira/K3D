#pragma once
#include "Engine/Source/Command/Core/InternalCommand.h"

namespace K3D {

	//���ԕ`�施�߃��X�g�Ǘ��p�N���X
	class InternalCommandAllocator
	{
		friend class CommandManager;

	public:

	private:

		std::list<InternalCommand> _allocatedMemory;

	public:

		InternalCommandAllocator();

		~InternalCommandAllocator();

		void Reset();

	private:
		

	};

}