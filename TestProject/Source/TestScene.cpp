#include "TestScene.h"



TestScene::TestScene() :
	Scene(K3D::Framework::GetInstance().GetRenderingManagre().GetRenderingDevice())

{
	_cube = std::unique_ptr<K3D::Cube>(new K3D::Cube(_gameHeap));
	_cube->Initialize();
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
	//test 
	static float time = 0.0f;
	time += 0.01f;
	_cube->GetTransform().SetPos(K3D::Vector3(0.0f, sinf(time), 0.0f));
	_cube->Update();

	std::stringstream ss;
	auto& logger = K3D::SystemLogger::GetInstance();

	logger.Log(K3D::LOG_LEVEL::Debug, ss.str());
}

void TestScene::Rendering()
{
	std::shared_ptr<K3D::CommandList> list;
	auto hr = _renderContext->CreateCommandList(_renderingDevice->GetD3D12Device(), D3D12_COMMAND_LIST_TYPE_DIRECT, list);

	_gameHeap->SetGameHeap(list);
	_renderContext->GetSwapChain()->SetRenderTarget(
		list, &_mainCamera->GetDepthStencil().GetDSVHeapPtr().GetCPUHandle(0)
	);
	list->RSSetViewports(1, &_mainCamera->GetViewport());
	list->RSSetScissorRects(1, &_mainCamera->GetScissorRect());

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
	//�O���t�B�b�N�f�o�b�O�c�[���ɂ��V�F�[�_�[�̃f�o�b�O�̗L��������
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

	//���_���̓��C�A�E�g�̒�`
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,		  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//���X�^���C�U�X�e�[�g�̐ݒ�
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
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
	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_BLEND_DESC descBS;
	//�����ꖢ�����N���b�v
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
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

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	psoDesc.DepthStencilState.DepthEnable = TRUE;								//�[�x�e�X�g����
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//�X�e���V���e�X�g�Ȃ�
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	this->_pso = std::make_shared<K3D::PipelineStateObject>();
	this->_rs = std::make_shared<K3D::RootSignature>();
	_rs->CreateFromShader(vs.Get());
	this->_pso->Initialize("PostEffectTestPSO", psoDesc, _rs);

}

void TestScene::InitializeRS()
{
}

void TestScene::CreateShader()
{
}