#include "TemporarilyBuffer.h"
#include "Engine/Source/Resource/Resource.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/DescriptorHeap/DescriptorHeap.h"


K3D::TemporarilyBuffer::TemporarilyBuffer()
{

}

K3D::TemporarilyBuffer::TemporarilyBuffer(Microsoft::WRL::ComPtr<ID3D12DeviceChild> pObj, int lifeTime)
{
	this->_pObj = pObj;
}

K3D::TemporarilyBuffer::TemporarilyBuffer(CommandList * pObj, int lifeTime)
{
	this->_pObj = pObj->GetCommandList();
}

K3D::TemporarilyBuffer::TemporarilyBuffer(Resource * pObj, int lifeTime)
{
	this->_pObj = pObj->GetResource();
}

K3D::TemporarilyBuffer::TemporarilyBuffer(DescriptorHeap * pObj, int lifeTime)
{
	this->_pObj = pObj->GetHeap();
}

K3D::TemporarilyBuffer::~TemporarilyBuffer()
{

}

void K3D::TemporarilyBuffer::TimeDeclement()
{
	--_lifeTime;
}

void K3D::TemporarilyBuffer::Discard()
{
	if (_pObj.Get() != nullptr) {
		this->_pObj.Reset();
	}
}
