#include "InternalCommandCreater.h"

K3D::InternalCommandCreater::InternalCommandCreater()
{
}

K3D::InternalCommandCreater::~InternalCommandCreater()
{

}

void K3D::InternalCommandCreater::SetThreadID(std::thread::id threadID)
{
	_threadID = threadID;
}

std::thread::id & K3D::InternalCommandCreater::GetThreadID()
{	
	return this->_threadID;
}

void K3D::InternalCommandCreater::SetThisThreadID()
{
	this->_threadID = std::this_thread::get_id();
}
