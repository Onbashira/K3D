#pragma once
namespace K3D {

	class Resource;
	class RenderContext;
	class RenderingPassHolder;
	class RenderingDevice;

	//レンダリングパスベース
	class RenderingPass
	{
	
	public:

	protected:

		std::vector<std::shared_ptr<Resource>> _prePathRenderTarges;

	private:

		String _pathName;

		std::weak_ptr<RenderingPassHolder> _parentHolder;

	public:

		RenderingPass(std::shared_ptr<RenderingPassHolder> parentHolder);

		virtual ~RenderingPass();

		virtual void FrameStartUpdate() = 0;

		virtual void PreExcuteUpdate() = 0;

		virtual void ExecutePath(std::shared_ptr<RenderContext>& renderingContext) = 0;

		virtual String& GetName() = 0;

		virtual int GetPathIndex() = 0;

	private:

	};

}
