#pragma once
#include "Engine/Source/Utility/ManagerComponent.h"
#include "Engine/Source/Command/Core/InternalCommand.h"
#include <list>

namespace K3D {
	class CommandManager
	{
	public:

	private:

		//���ԕ`��R�}���h���X�g
		std::list<InternalCommand> command;

	public:
		
		CommandManager();
		
		~CommandManager();
		
		void RecordCommand(InternalCommand& command);

	private:
	};
}