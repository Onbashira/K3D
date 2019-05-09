#pragma once
namespace K3D {

	class ParticleItem;


	struct EmitterHeader {
		size_t EmitterBinHead = 0;
		size_t ParticleBinHead = 0;
		size_t ParticleIdxHead = 0;
		unsigned int ParticleSize = 0;
	};

	class Emitter
	{
	public:

	private:

		EmitterHeader _header;

		std::function<void()> _updater;

		std::vector < std::unique_ptr<ParticleItem> >_items;

		size_t _dataSize;

	public:

		Emitter();

		~Emitter();

		void UpdateItems();

		void SetCustomUpdater(std::function<void(void)> function);

		const EmitterHeader& GetEmitterHeader();

		const size_t& GetBinSize();

		std::vector < std::unique_ptr<ParticleItem> >& GetEmitterItems();

		template <class T>
		Emitter& AddItems()
		{
			_items.push_back(std::make_unique<T>());
			_dataSize += sizeof(T);
			return *this;
		};

		void Discard();

	};

}