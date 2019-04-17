#pragma once
#include "Engine/Source/Scene/Scene.h"

namespace K3D {
	class Sprite;
	class PrimitiveObject;
	class RootSignature;
	class PipelineStateObject;
	class CommandList;
}
class TestScene : public K3D::Scene
{
private:

	std::unique_ptr<K3D::Sprite> _sprite;
	std::shared_ptr<K3D::RootSignature> _rs;
	std::shared_ptr<K3D::PipelineStateObject> _pso;
public:

	TestScene();

	virtual ~TestScene();

	virtual void Update();

	virtual void Rendering();

private:
	
	void InitializePSO();

	void InitializeRS();

	void CreateShader();


};

