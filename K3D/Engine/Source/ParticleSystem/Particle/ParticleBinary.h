#pragma once
namespace K3D {

	struct ParticleHeaders { //64bit

		bool Alive : 1;
		unsigned int EmitterID : 13;
		unsigned int ParticleIndex : 18;

		float Depth;
	};

	//GPU内でのみ読み書きされるパーティクル用メモリを管理
	class ParticleBinary
	{
	public:
		ParticleBinary();
		~ParticleBinary();
	};

}