#pragma once
namespace K3D {

	class ParticleItem;

	//�G�~�b�^���ʂ̃A�C�e��(CPU���j
	struct CPUEmitterHeader {
		//�G�~�b�^�̈�̃w�b�h�A�h���X
		unsigned int EmitterBinHead = 0;
		//�G�~�b�^�̈�̃T�C�Y
		unsigned int EmmiterBinSize = 0;
		//�p�[�e�B�N���o�C�i���̃w�b�_�A�h���X
		unsigned int ParticleBinHead = 0;
		//�p�[�e�B�N���o�C�i���̃T�C�Y
		unsigned int ParticleBinSize = 0;
		//�p�[�e�B�N���C���f�b�N�X�ێ��̈�̃C���f�b�N�X�ԍ�
		unsigned int ParticleIdxHead = 0;
		//�p�[�e�B�N���̃T�C�Y
		unsigned int ParticleSize = 0;

		CPUEmitterHeader() : EmitterBinHead(0), EmmiterBinSize(0),
			ParticleBinHead(0), ParticleIdxHead(0), ParticleSize(0) {};

		~CPUEmitterHeader() {};
	};

	struct GPUEmitterHeader {
		//�G�~�b�^�̗̈�̃w�b�h�A�h���X
		unsigned int EmitterBinHead = 0;
		//�p�[�e�B�N���o�C�i���̃w�b�_�A�h���X
		unsigned int ParticleBinHead = 0;
		//�p�[�e�B�N���C���f�b�N�X�ێ��̈�̃C���f�b�N�X�ԍ�
		unsigned int ParticleIdxHead = 0;
		//�p�[�e�B�N���̃T�C�Y
		unsigned int ParticleSize = 0;

		GPUEmitterHeader() : EmitterBinHead(),
			ParticleBinHead(), ParticleIdxHead(), ParticleSize() {};
		GPUEmitterHeader() : EmitterBinHead(),
			ParticleBinHead(), ParticleIdxHead(), ParticleSize() {};
		~GPUEmitterHeader() {};
	};

	//�G�~�b�^�f�[�^�iGPU���ł̂ݓǂݏ�������郁�����̃t�H�[�}�b�g
	struct EmitterParam {
		//�������ʒu
		unsigned int SpawnHead = 0;
		//�X�|�[���ő吔
		unsigned int SpawnMax = 0;
		//�p�[�e�B�N����
		unsigned int ParticleNum = 0;
	};

	class Emitter
	{
	public:

	private:

		//�A�C�e���A�b�v�f�[�^
		std::function<void()> _updater;

		//�A�C�e���z��
		std::vector < std::unique_ptr<ParticleItem> >_items;

		//�G�~�b�^�A�C�e�����̃T�C�Y�iByte)
		unsigned int _dataSize;

		EmitterParam _emitterData;

	public:

		Emitter();

		~Emitter();

		//�e�A�C�e���̃A�b�v�f�[�g
		void UpdateItems();

		//�A�C�e���A�b�v�f�[�^�̕ύX
		void SetCustomUpdater(std::function<void(void)> function);

		//�G�~�b�^�̏��̃Z�b�g
		void SetEmitterParam(const EmitterParam& emitterData);

		//�A�C�e�����T�C�Y�̎擾
		const unsigned int& GetBinSize();

		//�G�~�b�^�A�C�e���z��̎擾
		std::vector < std::unique_ptr<ParticleItem> >& GetEmitterItems();

		template <class T>
		Emitter& AddItems()
		{
			_items.push_back(std::make_unique<T>());
			_dataSize += static_cast<unsigned int>(sizeof(T));
			return *this;
		};

		void Discard();

	};

}