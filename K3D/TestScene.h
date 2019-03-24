#pragma once
#include "Engine/Source/Scene/Scene.h"

namespace K3D {
	class Cube;
	class PrimitiveObject;
	class RootSignature;
	class PipelineStateObject;
	class CommandList;
}
class TestScene : public K3D::Scene
{
private:

	std::unique_ptr<K3D::Cube> _cube;

	std::shared_ptr<K3D::RootSignature> _rs;
	std::shared_ptr<K3D::PipelineStateObject> _pso;

	std::shared_ptr<K3D::CommandList> _cmdList;

public:

	TestScene();

	virtual ~TestScene();

	virtual void ScreenClear();

	virtual void Update();

	virtual void Rendering();

	virtual void ScreenFlip();

	virtual void BeginRenderPath();

	virtual void EndRenderPath();

private:
	
	void InitializePSO();

	void InitializeRS();

	void CreateShader();


};

