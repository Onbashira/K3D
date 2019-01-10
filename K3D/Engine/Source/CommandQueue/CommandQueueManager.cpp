#include "CommandQueueManager.h"
#include "Engine/Source/CommandQueue/CommandQueue.h"

K3D::CommandQueueManager::CommandQueueManager()
{
}

K3D::CommandQueueManager::~CommandQueueManager()
{
	Discard();
}

HRESULT K3D::CommandQueueManager::Create(std::string name, D3D12_COMMAND_QUEUE_DESC& desc)
{
	auto res = std::make_shared<CommandQueue>();
	res->SetName(name);
	auto hr = res->Create(desc);
	CHECK_RESULT(hr);
	this->_queues.Set(name, res);
	return hr;
}

std::weak_ptr<K3D::CommandQueue> K3D::CommandQueueManager::Get(std::string name)
{
	return this->_queues.Get(name);
}

void K3D::CommandQueueManager::Erase(std::string name)
{
	this->_queues.Erase(name);
}

void K3D::CommandQueueManager::DiscardMap()
{
	this->_queues.DiscardMap();
}

bool K3D::CommandQueueManager::IsLooded(std::string name)
{
	return this->_queues.IsLooded(name);
}

void K3D::CommandQueueManager::Discard()
{
	this->_queues.DiscardMap();
}
