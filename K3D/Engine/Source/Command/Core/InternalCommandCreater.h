#pragma once
#include "InternalCommand.h"
#include <list>
namespace K3D {
	struct InternalCommand;
	//中間命令を作る機能を持つ
	class InternalCommandCreater
	{
	public:
	private:
		//自身が生成されたthreadのID
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