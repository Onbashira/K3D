#pragma once
namespace K3D {

	class RenderContext;
	class RenderingPassHolder;
	class Render;
	class GameHeap;

	class Scene
	{

		friend class RenderingManager;
		
	public:
	private:

		std::unique_ptr<RenderContext> _renderCotext;
		std::unique_ptr<RenderingPassHolder> _renderCotext;
		std::shared_ptr<GameHeap> _gameHeap;

	public:
		
		Scene();
		
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