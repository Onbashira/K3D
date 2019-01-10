#pragma once
#include "InternalCommand.h"
#include <list>
namespace K3D {
	struct InternalCommand;
	//’†ŠÔ–½—ß‚ğì‚é‹@”\‚ğ‚Â
	class InternalCommandCreater
	{
	public:
	private:
		//©g‚ª¶¬‚³‚ê‚½thread‚ÌID
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