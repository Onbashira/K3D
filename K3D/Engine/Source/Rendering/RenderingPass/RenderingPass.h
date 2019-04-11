#pragma once
namespace K3D {

	class Resource;
	class RenderContext;
	class RenderingPassHolder;
	class RenderingDevice;
	class PipelineStateObject;
	class RootSignature;
	class CommandList;
	
	//レンダリングパス
	//レンダリングパイプラインのパスを定義

	class RenderingPass
	{
	
	public:

	protected:

		std::vector<std::shared_ptr<Resource>> _prePathRenderTarges;

		std::weak_ptr<RenderingPassHolder> _parentHolder;

		std::shared_ptr<PipelineStateObject> _pipelineStateObject;
		//パスが持つコマンドリスト
		std::shared_ptr<CommandList> _cmdList;

		std::weak_ptr<RenderContext> _renderContext;

		std::shared_ptr<RenderingDevice> _renderingDevice;
	private:

		String _passName;

	public:

		RenderingPass(std::shared_ptr<RenderingPassHolder> parentHolder, std::shared_ptr<RenderContext>& renderContext, std::shared_ptr<RenderingDevice>& renderingDevice);

		virtual ~RenderingPass();

		virtual void FrameStartUpdate() = 0;

		virtual void PreExcuteUpdate() = 0;

		virtual String& GetName() = 0;

		virtual std::weak_ptr<CommandList> GetPassCmdList();

		void SetPathName(const String& name);

		virtual void Discard();

	private:

	};

}
