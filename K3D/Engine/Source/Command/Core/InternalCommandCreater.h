#pragma once
#include "InternalCommand.h"
#include <list>
namespace K3D {
	struct InternalCommand;
	//���Ԗ��߂����@�\������
	class InternalCommandCreater
	{
	public:
	private:
		//���g���������ꂽthread��ID
		unsigned int _threadID;
		//
		std::list<InternalCommand> _commands;
	public:
	
		InternalCommandCreater();
	
		~InternalCommandCreater();

		void CreateCommand();

		void AddCommand();

		void AddCommands();

		void ClearCommands();



	private:

		void SortCommand();

	};

}