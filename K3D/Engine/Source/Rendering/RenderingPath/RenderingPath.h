#pragma once
namespace K3D {

	class Resource;
	class RenderingPath
	{
	public:
	private:
	public:

		RenderingPath();

		~RenderingPath();

		virtual void ExecutePath() = 0;

		virtual String& GetName() = 0;

		virtual int GetPathIndex() = 0;




	private:
	};

}
