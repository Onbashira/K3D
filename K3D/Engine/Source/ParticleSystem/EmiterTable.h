#pragma once
namespace K3D {

	struct EmitterHeader {
		int index;
		size_t emitterBinaryHead;
		size_t particleBinaryHead;
		size_t particleSize;

	};

	class EmiterTable
	{
	public:
	private:
		std::map<int, int> _table;
		std::map<int, size_t> _memoryBynarySize;
	public:
		EmiterTable();
		~EmiterTable();
	};

}