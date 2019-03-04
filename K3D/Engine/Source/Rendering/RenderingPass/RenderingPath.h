#pragma once
namespace K3D {

	class Resource;

	//レンダリングパスベース
	class RenderingPass
	{
	public:

	private:

		String _pathName;

		std::vector<std::shared_ptr<Resource>> _prePathRenderTarges;

	public:

		RenderingPass();

		~RenderingPass();

		virtual void ExecutePath() = 0;

		virtual String& GetName() = 0;

		virtual int GetPathIndex() = 0;

		virtual std::vector<std::shared_ptr<Resource> >& GetPrePathRenderTargets() { _prePathRenderTarges; };

	private:
		
	};

}
