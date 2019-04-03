#include "stdafx.h"
#include "RenderingPathHolder.h"
#include "RenderingPass.h"


K3D::RenderingPathHolder::RenderingPathHolder(std::shared_ptr<GameHeap>& gameHeap) :
	_gameHeap(gameHeap)
{
}

K3D::RenderingPathHolder::~RenderingPathHolder()
{
	ClearRenderingPasses();
}

void K3D::RenderingPathHolder::RenderingBegin()
{

}

void K3D::RenderingPathHolder::RenderingEnd()
{
}

void K3D::RenderingPathHolder::AddRenderingPass(std::shared_ptr<RenderingPass> pass)
{
	this->_rpList.push_back(pass);
}

void K3D::RenderingPathHolder::ClearRenderingPasses()
{
	for (auto& pass : this->_rpList) {
		pass->Discard();

	}
}

void K3D::RenderingPathHolder::InsertRenderingPass(int index, std::shared_ptr<RenderingPass> pass)
{
	auto begin = _rpList.begin();
	begin += index;
	this->_rpList.insert(begin, pass);
}
