#pragma once
namespace K3D {

	class GameObject;

	class InputComponent
	{
	public:
	
	private:
	
	public:
		
		InputComponent();
		
		virtual ~InputComponent();
	
		virtual void Update(GameObject* gameObject) = 0;

	private:
	
	};
}