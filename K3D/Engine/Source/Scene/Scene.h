#pragma once
namespace K3D {

	class RenderContext;
	class RenderingDevice;
	class RenderingPassHolder;
	class GameHeap;
	class Camera;

	//シーンを構成する基底クラス
	class Scene
	{

		friend class RenderingManager;
		
	public:
	protected:
		//!レンダリングパスを保持する
		std::shared_ptr<RenderingPassHolder> _pathHolder;
		//!デスクリプタヒープ
		std::shared_ptr<GameHeap> _gameHeap;

		std::shared_ptr<RenderingDevice>& _renderingDevice;

		std::shared_ptr<RenderContext> _renderContext;

		std::shared_ptr<Camera> _mainCamera;
	private:
	public:
		
		Scene(std::shared_ptr<RenderingDevice>& renderingDevice);
		
		~Scene();

		virtual void SceneBegin();

		virtual void Update() = 0;

		virtual void Rendering() = 0;

		virtual void SceneEnd();

		virtual void Discard();

	private:

	};

}