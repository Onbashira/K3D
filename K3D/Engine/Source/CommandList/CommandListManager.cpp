#include "CommandList.h"
#include "Engine/Source/CommandList/CommandListManager.h"
#include "Engine/Source/Debug/Logger.h"


K3D::CommandListManager::CommandListManager()
{

}


K3D::CommandListManager::~CommandListManager()
{
	Discard();
}


HRESULT K3D::CommandListManager::Create(std::string commandListName,unsigned int nodeMask, D3D12_COMMAND_LIST_TYPE listType)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName] = std::make_shared<CommandList>();
		auto hr = this->_library[commandListName]->Initialize(nodeMask, listType);
		this->_library[commandListName]->SetName(commandListName);
		if (SUCCEEDED(hr)) {
			DEBUG_LOG(std::string( "CommandList������ɍ쐬�E�o�^����܂��� : " + commandListName));
			return hr;
		}
		return hr;
	}
	return E_FAIL;
}

void K3D::CommandListManager::Set(std::string commandListName, std::shared_ptr<CommandList> commandList)
{
	if (_library.find(commandListName) == _library.end()) {
		this->_library[commandListName].reset();
		this->_library[commandListName] = commandList;
		DEBUG_LOG(std::string("CommandList������ɍ쐬�E�o�^����܂��� : " + commandListName));

	}
}

std::shared_ptr<K3D::CommandList> K3D::CommandListManager::Get(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		return  this->_library[commandListName];
	}
	ERROR_LOG(std::string("�s���������[�h�ł� : " + commandListName));

	return nullptr;
}

void K3D::CommandListManager::Erase(std::string commandListName)
{
	if (_library.find(commandListName) != _library.end()) {
		this->_library.erase(commandListName);
		DEBUG_LOG(std::string("CommandList������ɍ폜����܂��� : " + commandListName));

	}
}

void K3D::CommandListManager::Discard()
{
	for (auto& list : _library) {
		list.second->Discard();
		list.second.reset();
	}
	_library.clear();
}

