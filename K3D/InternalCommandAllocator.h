#pragma once
namespace K3D {
	class InternalCommand;
	class InternalCommandAllocator
	{

	public:

	private:

		std::list<InternalCommand> _allocatedMemory;

	public:

		InternalCommandAllocator();

		~InternalCommandAllocator();

	private:
		
		void Reset();

	};

}