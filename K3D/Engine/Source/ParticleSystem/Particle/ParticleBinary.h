#pragma once
namespace K3D {

	struct ParticleHeaders { //64bit

		bool Alive : 1;
		unsigned int EmitterID : 13;
		unsigned int ParticleIndex : 18;

		float Depth;
	};

	//GPU���ł̂ݓǂݏ��������p�[�e�B�N���p���������Ǘ�
	class ParticleBinary
	{
	public:
		ParticleBinary();
		~ParticleBinary();
	};

}