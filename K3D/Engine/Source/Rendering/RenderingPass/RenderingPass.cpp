#include "stdafx.h"
#include "RenderingPass.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Device/RenderingDevice.h"


K3D::RenderingPass::RenderingPass(std::shared_ptr<RenderingPassHolder> parentHolder,
	std::shared_ptr<RenderContext>& renderContext, std::shared_ptr<RenderingDevice>& renderingDevice) :
	_parentHolder(parentHolder),
	_renderContext(renderContext),
	_renderingDevice(renderingDevice),
	_passName("UnNamed Pass")
{
	if (FAILED(renderContext->CreateCommandList(renderingDevice->GetD3D12Device(),
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		_cmdList)))
	{
		Util::Comment(L"RenderingPass‚Ìì¬‚É¸”s‚µ‚Ü‚µ‚½ ƒvƒƒOƒ‰ƒ€‚ğ‹­§I—¹‚µ‚Ü‚·");
		exit(-1);
	}
}

K3D::RenderingPass::~RenderingPass()
{
	Discard();
}

std::weak_ptr<K3D::CommandList> K3D::RenderingPass::GetPassCmdList()
{
	return _cmdList;
}

void K3D::RenderingPass::SetPathName(const String & name)
{
	_passName = name;
}

void K3D::RenderingPass::Discard()
{
	if (_prePathRenderTarges.size() > 0) {
		_prePathRenderTarges.clear();
		_prePathRenderTarges.shrink_to_fit();
		_passName = String("Discarded Pass");
		
	}
}


