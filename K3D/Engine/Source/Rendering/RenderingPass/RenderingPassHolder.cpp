#include "stdafx.h"
#include "RenderingPassHolder.h"
#include "RenderingPass.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"



K3D::RenderingPassHolder::RenderingPassHolder(std::shared_ptr<GameHeap>& gameHeap) :
	_gameHeap(gameHeap)
{
}

K3D::RenderingPassHolder::~RenderingPassHolder()
{
	ClearRenderingPasses();
}

void K3D::RenderingPassHolder::RenderingBegin()
{

}

void K3D::RenderingPassHolder::RenderingEnd()
{
}

std::pair<unsigned int, std::weak_ptr<K3D::CommandList>> K3D::RenderingPassHolder::AddRenderingPass(std::shared_ptr<RenderingPass> pass)
{
	this->_rpList.push_back(pass);
	unsigned int index = static_cast<unsigned int>(_rpList.size());
	std::weak_ptr<CommandList> list = pass->GetPassCmdList();
	return { index, list };
}

void K3D::RenderingPassHolder::ClearRenderingPasses()
{
	for (auto& pass : this->_rpList) {
		pass->Discard();

	}
}

void K3D::RenderingPassHolder::InsertRenderingPass(int index, std::shared_ptr<RenderingPass> pass)
{
	auto begin = _rpList.begin();
	begin += index;
	this->_rpList.insert(begin, pass);
}

std::weak_ptr<K3D::CommandList> K3D::RenderingPassHolder::GetRenderPassCmdList(int renderPassIndex)
{
	return _rpList[renderPassIndex]->GetPassCmdList();
}
