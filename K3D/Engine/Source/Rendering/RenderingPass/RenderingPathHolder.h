#pragma once


namespace K3D {

	class RenderingPass;

//レンダリングパスを保持するクラス
	class RenderingPathHolder
	{

	private:
		std::list<std::shared_ptr<RenderingPass>> _rpList;
	public:

		RenderingPathHolder();

		virtual ~RenderingPathHolder();

		void RegistRenderingPass(std::shared_ptr<RenderingPass> pass);

	};

}