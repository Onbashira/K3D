#pragma once
namespace K3D {

	class Resource;
	class Emitter;
	class D3D12Device;
	struct CPUEmitterHeader;
	struct EmitterParam;

	struct ParticleHeaders { //64bit

		bool Alive : 1;
		unsigned int EmitterID : 13;
		unsigned int ParticleIndex : 18;

		float Depth;
	};

	struct EmitterRange {
		unsigned int AliveHead;
		unsigned int AliveEnd;
		unsigned int DeadHead;
		unsigned int DeadEnd;

	};

	//GPU内でのみ読み書きされるパーティクル用メモリを管理
	class ParticleBinary
	{
	private:

		std::unique_ptr<K3D::Resource> _particleBin;

		std::unique_ptr<K3D::Resource> _particleHeadersBin;

		std::unique_ptr<K3D::Resource> _particleIndexListBin;

	public:

		ParticleBinary();

		~ParticleBinary();

		HRESULT Initialize(std::shared_ptr<D3D12Device>& device , unsigned int ptBinSize, unsigned int ptHeadersSize, unsigned ptIdxListSize);



		void Discard();

	private:

		HRESULT PtBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned int ptBinSize);

		HRESULT PtHeaderesBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned int ptHeadersSize);

		HRESULT PtIdxListBinInitialize(std::shared_ptr<D3D12Device>& device, unsigned ptIdxListSize);

	};

}