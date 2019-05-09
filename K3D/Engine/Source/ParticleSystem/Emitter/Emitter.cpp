#include "stdafx.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"

K3D::Emitter::Emitter() : 
	_header(),
	_updater([this]() {for (auto& item : this->_items) { item->UpdateItem(); }}),
	_dataSize(sizeof(EmitterHeader))
{

}


K3D::Emitter::~Emitter()
{
}

void K3D::Emitter::UpdateItems()
{
	_updater();
}

void K3D::Emitter::SetCustomUpdater(std::function<void(void)> function)
{
	_updater = function;
}

const K3D::EmitterHeader& K3D::Emitter::GetEmitterHeader()
{
	return _header;
}

const size_t & K3D::Emitter::GetBinSize()
{
	return _dataSize;
}

std::vector<std::unique_ptr<K3D::ParticleItem>>& K3D::Emitter::GetEmitterItems()
{
	return  _items;
}

void K3D::Emitter::Discard()
{
	_updater = []() {};
	_items.clear();
}
