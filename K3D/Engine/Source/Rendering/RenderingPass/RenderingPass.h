#pragma once
namespace K3D {

	class Resource;
	class RenderContext;
	class RenderingDevice;

	//レンダリングパスベース
	class RenderingPass
	{
	public:

	private:

		String _pathName;

		std::vector<std::shared_ptr<Resource>> _prePathRenderTarges;

	public:

		RenderingPass();

		virtual ~RenderingPass();

		virtual void FrameStartUpdate() = 0;

		virtual void PreExcuteUpdate() = 0;

		virtual void ExecutePath(std::shared_ptr<RenderContext>& renderingContext) = 0;

		virtual String& GetName() = 0;

		virtual int GetPathIndex() = 0;

	private:

	};

}
