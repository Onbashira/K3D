#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Device/D3D12Device.h"

K3D::Scene::Scene(std::shared_ptr<RenderingDevice> renderingDevice, std::shared_ptr<RenderContext> renderContext) : 
_renderingDevice(renderingDevice), _renderCotext(renderContext)
{

}

K3D::Scene::~Scene()
{
}
