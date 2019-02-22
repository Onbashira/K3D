#include "PipelineState.h"

K3D::PipelineState::PipelineState()
{
}

K3D::PipelineState::~PipelineState()
{
}


std::weak_ptr<K3D::PipelineStateObject> K3D::PipelineState::GetPSO()
{
	return this->_pso;
}

std::weak_ptr<K3D::RootSignature> K3D::PipelineState::GetRS()
{
	return this->_rs;
}

void K3D::PipelineState::SetPSO(std::weak_ptr<PipelineStateObject> pso)
{
 this->_pso = pso.lock();

}

void K3D::PipelineState::SetRS(std::weak_ptr<RootSignature> rs)
{
	this->_rs = rs.lock();
}
