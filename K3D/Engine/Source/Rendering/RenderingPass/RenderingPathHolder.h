#pragma once


namespace K3D {

	class RenderingPass;
	class GameHeap;

//レンダリングパスを保持するクラス
	class RenderingPathHolder
	{

	private:

		std::vector<std::shared_ptr<RenderingPass>> _rpList;

		std::shared_ptr<GameHeap>& _gameHeap;

	public:

		RenderingPathHolder(std::shared_ptr<GameHeap>& _gameHeap);

		virtual ~RenderingPathHolder();

		void RenderingBegin();

		void RenderingEnd();

		void AddRenderingPass(std::shared_ptr<RenderingPass> pass);

		void ClearRenderingPasses();

		void InsertRenderingPass(int index, std::shared_ptr<RenderingPass> pass);

	};

}