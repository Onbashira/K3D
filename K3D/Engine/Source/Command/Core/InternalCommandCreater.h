#pragma once
#include "InternalCommand.h"
#include <list>
namespace K3D {

	struct InternalCommand;

	struct NativeCommand;

	//���Ԗ��߂����@�\������
	class InternalCommandCreater
	{

	public:

	private:

		//���g���������ꂽthread��ID
		std::thread::id _threadID;

		//�R�}���h���X�g
		std::list<InternalCommand> _commands;

		//�~���[�e�b�N�X
		std::mutex _mutex;

	public:
	
		InternalCommandCreater();
	
		~InternalCommandCreater();
		 
		void SetThreadID(std::thread::id threadID);

		std::thread::id& GetThreadID();

		void SetThisThreadID();

		NativeCommand ConvertCommand();

	private:

	};

}