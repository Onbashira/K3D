#pragma once
#include "Engine/Source/Component/GameObject/GameObject.h"
namespace K3D {

	class Sprite :public GameObject
	{

	private:
	


	public:

		Sprite();

		~Sprite();

		virtual void Update() = 0;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) = 0;


	};

}