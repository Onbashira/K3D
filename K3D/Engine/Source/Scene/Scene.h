#pragma once
namespace K3D {

	class Scene
	{
	public:
	private:
	public:
		
		Scene();
		
		~Scene();
		virtual void ScreenClear() = 0;

		virtual void Update() = 0;

		virtual void Rendering() = 0;

		virtual void ScreenFlip() = 0;

	private:

	};

}