#include "stdafx.h"
#include "Scene.h"


K3D::Scene::Scene(std::shared_ptr<RenderingDevice> renderingDevice, std::shared_ptr<RenderContext> renderContext) : 
_renderingDevice(renderingDevice), _renderCotext(renderContext)
{

}

K3D::Scene::~Scene()
{
}
