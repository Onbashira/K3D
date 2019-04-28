#include "AudioSourceManager.h"


void K3D::AudioSourceManager::SetResource(std::string resourceName,std::shared_ptr<AudioWaveSource> source)
{
	this->_resourceMap.Set(resourceName,source);
}

std::weak_ptr<K3D::AudioWaveSource> K3D::AudioSourceManager::GetResource(std::string resourceName)
{
	return this->_resourceMap.Get(resourceName);
}

void K3D::AudioSourceManager::EraseResource(std::string name)
{
	this->_resourceMap.Erase(name);
}

bool K3D::AudioSourceManager::IsLoaded(std::string resourceName)
{
	return this->_resourceMap.IsLooded(resourceName);
}

void K3D::AudioSourceManager::DiscardManager()
{
	this->_resourceMap.DiscardMap();
}
