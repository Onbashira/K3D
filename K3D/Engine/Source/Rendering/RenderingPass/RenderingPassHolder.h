#pragma once


namespace K3D {

	class RenderingPass;
	class RenderContext;
	class CommandList;
	class GameHeap;

//レンダリングパスを保持するクラス
	class RenderingPassHolder
	{

	private:

		std::vector<std::shared_ptr<RenderingPass>> _rpList;

		std::shared_ptr<GameHeap>& _gameHeap;
		

	public:

		RenderingPassHolder(std::shared_ptr<GameHeap>& _gameHeap);

		virtual ~RenderingPassHolder();

		virtual void RenderingBegin();

		virtual void RenderingEnd();

		std::pair<unsigned int, std::weak_ptr<CommandList>> AddRenderingPass(std::shared_ptr<RenderingPass> pass);

		void ClearRenderingPasses();

		void InsertRenderingPass(int index, std::shared_ptr<RenderingPass> pass);

		std::weak_ptr<K3D::CommandList> GetRenderPassCmdList(int renderPassIndex);

	};

}