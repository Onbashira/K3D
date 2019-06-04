#include "stdafx.h"
#include "Emitter.h"
#include "../Item/ParticleItem.h"

K3D::Emitter::Emitter() : 
	_updater([this]() {for (auto& item : this->_items) { item->UpdateItem(); }}),
	_dataSize(sizeof(EmitterCommonItem))
{

}


K3D::Emitter::~Emitter()
{
	Discard();
}

void K3D::Emitter::UpdateItems()
{
	_updater();
}

void K3D::Emitter::SetCustomUpdater(std::function<void(void)> function)
{
	_updater = function;
}

void K3D::Emitter::SetEmitterParam(const EmitterParam & emitterData)
{
	_emitterData = emitterData;
}

const unsigned int & K3D::Emitter::GetBinSize()
{
	return _dataSize;
}

K3D::EmitterCommonItem & K3D::Emitter::GetCommonItem()
{
	return _emitterCommonItem;
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
