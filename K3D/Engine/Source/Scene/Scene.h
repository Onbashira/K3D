#pragma once
namespace K3D {

	class Scene
	{
	public:
	private:
	public:
		
		Scene();
		
		~Scene();

		virtual void Update() = 0;

		virtual void Rendering() = 0;

	private:

	};

}