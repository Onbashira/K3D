#include "stdafx.h"
#include "Descriptor.h"
#include "Engine/Source/CommandList/CommandList.h"

void BindCPUDescriptor(unsigned int rootParam,K3D::Descriptor::BindType bindType ,K3D::Descriptor& descriptor, std::shared_ptr<K3D::CommandList> list);

void K3D::Descriptor::BindShader(unsigned int rootParamater, BindType bindType, std::shared_ptr<K3D::CommandList> list)
{
	switch (viewType)
	{

	case K3D::Descriptor::ViewType::CBV:
	case K3D::Descriptor::ViewType::SRV:
	case K3D::Descriptor::ViewType::UAV:
	case K3D::Descriptor::ViewType::SAMP:

		BindCPUDescriptor(rootParamater, bindType, *this, list);
		break;
	case K3D::Descriptor::ViewType::RTV:
	case K3D::Descriptor::ViewType::DSV:
		
		break;

	case K3D::Descriptor::ViewType::EMPTY:
		//none
		break;
	default:
		break;
	}
}

void BindCPUDescriptor(unsigned int rootParam, K3D::Descriptor::BindType bindType, K3D::Descriptor & descriptor, std::shared_ptr<K3D::CommandList> list)
{
	switch (bindType)
	{
	case K3D::Descriptor::BindType::Graphics:
		list->SetGraphicsRootDescriptorTable(rootParam, descriptor.gpuAddress);
		break;
	case K3D::Descriptor::BindType::Compute:
		list->SetComputeRootDescriptorTable(rootParam, descriptor.gpuAddress);
		break;
	default:
		break;
	}
}
