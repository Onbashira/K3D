#pragma once


namespace K3D {

	class RenderingPass;
	class GameHeap;

//�����_�����O�p�X��ێ�����N���X
	class RenderingPathHolder
	{

	private:

		std::list<std::shared_ptr<RenderingPass>> _rpList;

		std::shared_ptr<GameHeap> _gameHeap;

	public:

		RenderingPathHolder();

		virtual ~RenderingPathHolder();

		void AddRenderingPass(std::shared_ptr<RenderingPass> pass);

	};

}