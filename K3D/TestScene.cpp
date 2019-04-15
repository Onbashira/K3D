#include "stdafx.h"
#include "TestScene.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/Primitive/Geometry/Cube.h"
#include "Engine/Source/Camera/Camera.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderingManager.h"
#include "Engine/Source/Rendering/RenderingPass/RenderingPass.h"
#include "Engine/Source/Rendering/RenderingPass/RenderingPassHolder.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/CoreSystem/Framework.h"
#include "Engine/Source/ShaderObject/ShaderObjectManager.h"
#include "Engine/Source/ShaderObject/ShaderHelper.h"
#include "Engine/Source/Signature/RootSignature.h"
#include "Engine/Source/PipelineState/PipelineStateObject.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"


TestScene::TestScene() :
	Scene(K3D::Framework::GetInstance().GetRenderingManagre().GetRenderingDevice())

{
	_cube = std::unique_ptr<K3D::Cube>(new K3D::Cube(_gameHeap));
	_cube->Initialize();
	_cube->GetTransform().SetScale(K3D::Vector3(2.0f, 2.0f, 2.0f));
	_cube->Update();
	InitializePSO();
}


TestScene::~TestScene()
{
}

void TestScene::Update()
{
	_mainCamera->DebugMove(K3D::Framework::GetInstance().Input());
	_mainCamera->DebugRotate(K3D::Framework::GetInstance().Input());
	_mainCamera->Update();
}

void TestScene::Rendering()
{
	std::shared_ptr<K3D::CommandList> list;
	auto hr = _renderContext->CreateCommandList(_renderingDevice->GetD3D12Device(), D3D12_COMMAND_LIST_TYPE_DIRECT, list);

	_gameHeap->SetGameHeap(list);
	_renderContext->GetSwapChain()->SetRenderTarget(
		list, &_mainCamera->GetDepthStencil().GetDSVHeapPtr().GetCPUHandle(0)
	);
	list->SetGraphicsRootSignature(_rs);
	list->SetPipelineState(_pso);
	_cube->Draw(list);
	list->CloseCommandList();


}

void TestScene::InitializePSO()
{

	HRESULT ret = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> vs;
	Microsoft::WRL::ComPtr<ID3DBlob> gs;
	Microsoft::WRL::ComPtr<ID3DBlob> ds;
	Microsoft::WRL::ComPtr<ID3DBlob> hs;
	Microsoft::WRL::ComPtr<ID3DBlob> ps;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

#if defined(_DEBUG)
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif
	//K3D::HLSLIncluder includes("./Engine/Shader/");

	K3D::ShaderHelper shaderHelper;
	ret = shaderHelper.CompileShader(K3D::ShaderHelper::SHADER_TYPE::SHADER_TYPE_VERTEX,
		"./Engine/Shader/Shader/PrimitiveTestShader.hlsl", "VSMain", "vs_5_0");
	ret = shaderHelper.CompileShader(K3D::ShaderHelper::SHADER_TYPE::SHADER_TYPE_PIXEL,
		"./Engine/Shader/Shader/PrimitiveTestShader.hlsl", "PSMain", "ps_5_0");

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL"	,  0, DXGI_FORMAT_R32G32B32_FLOAT,	  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//ラスタライザステートの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = FALSE;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	descRTBS.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	descRTBS.BlendOp = D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	descRTBS.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_DEST_ALPHA;
	descRTBS.LogicOp = D3D12_LOGIC_OP_CLEAR;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	//αが一未満をクリップ
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//パイプラインステートオブジェクト
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		vs = shaderHelper.GetShader(K3D::ShaderHelper::SHADER_TYPE::SHADER_TYPE_VERTEX);
		shaderBytecode.BytecodeLength = vs->GetBufferSize();
		shaderBytecode.pShaderBytecode = vs->GetBufferPointer();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		ps = shaderHelper.GetShader(K3D::ShaderHelper::SHADER_TYPE::SHADER_TYPE_PIXEL);
		shaderBytecode.BytecodeLength = ps->GetBufferSize();
		shaderBytecode.pShaderBytecode = ps->GetBufferPointer();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//深度テストあり
	psoDesc.DepthStencilState.DepthEnable = true;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.StencilEnable = false;

	this->_pso = std::make_shared<K3D::PipelineStateObject>();
	this->_rs = std::make_shared<K3D::RootSignature>();
	_rs->CreateFromShader(vs.Get());
	this->_pso->Initialize("PrimitiveTestShaderPSO", psoDesc, _rs);

}

void TestScene::InitializeRS()
{
}

void TestScene::CreateShader()
{
}
