#pragma once
namespace K3D {

	class RenderContext;
	class RenderingDevice;
	class RenderingPassHolder;
	class GameHeap;

	//シーンを構成する基底クラス
	class Scene
	{

		friend class RenderingManager;
		
	public:
	private:
		//!レンダリングパスを保持する
		std::shared_ptr<RenderingPassHolder> _pathHolder;
		//!デスクリプタヒープ
		std::shared_ptr<GameHeap> _gameHeap;
		std::shared_ptr<RenderingDevice> _renderingDevice;
		std::shared_ptr<RenderContext> _renderCotext;
		String _name;
	public:
		
		Scene(std::shared_ptr<RenderingDevice> renderingDevice , std::shared_ptr<RenderContext> renderCotext);
		
		~Scene();

		virtual void ScreenClear() = 0;

		virtual void Update() = 0;

		virtual void Rendering() = 0;

		virtual void ScreenFlip() = 0;

		virtual void BeginRenderPath() = 0;

		virtual void EndRenderPath() = 0;

	private:

	};

}