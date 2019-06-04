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

	enum EmitterState {
		EMITTER_STATE_START = 0,
		EMITTER_STATE_UPDATE,
		EMITTER_STATE_DEAD
	};

	enum EmitterActiveItem {
		EMITTER_ACITVE_ITEM_NONE = 0x0000,
		EMITTER_ACITVE_ITEM_SHAPE_3D = 0x0001,
		EMITTER_ACITVE_ITEM_PARENT_OPTION = 0x0002,
		EMITTER_ACITVE_ITEM_TURBULEANCE = 0x0004,
		EMITTER_ACITVE_ITEM_VELOCITY = 0x0008,
		EMITTER_ACITVE_ITEM_SCALE_ANIM = 0x0010,
		EMITTER_ACITVE_ITEM_ROTATE_ANIM = 0x0020,
		EMITTER_ACITVE_ITEM_NOISE = 0x0040,
		EMITTER_ACITVE_ITEM_BLINK = 0x0080,
		EMITTER_ACITVE_ITEM_BILLBORD_3D = 0x0100,
		EMITTER_ACITVE_ITEM_RIBBON = 0x0200,
		EMITTER_ACITVE_ITEM_FLOW_MAP = 0x0400,
		EMITTER_ACITVE_ITEM_UV_SEQUENCE = 0x0800,
		EMITTER_ACITVE_ITEM_LIFE = 0x1000,
		EMITTER_ACITVE_ITEM_RGB_FILTER = 0x2000,
		EMITTER_ACITVE_ITEM_RGB_WATER = 0x4000,

	};

	struct EmitterCommonItem {
		EmitterState State;
		EmitterActiveItem ActiveItemState;
		Vector3 Pos;
		Vector3 Scale;
		Quaternion Rotate;

	};

	class Emitter
	{

	public:

	protected:

		//�A�C�e���A�b�v�f�[�^
		std::function<void()> _updater;

		//���ʃA�C�e��
		EmitterCommonItem _emitterCommonItem;

		//�A�C�e���z��
		std::vector < std::unique_ptr<ParticleItem> >_items;

		//�G�~�b�^�A�C�e�����̃T�C�Y�iByte)
		unsigned int _dataSize;

		EmitterParam _emitterData;

	public:

		~Emitter();

		//�e�A�C�e���̃A�b�v�f�[�g
		void UpdateItems();

		//�A�C�e���A�b�v�f�[�^�̕ύX
		void SetCustomUpdater(std::function<void(void)> function);

		//�G�~�b�^�̏��̃Z�b�g
		void SetEmitterParam(const EmitterParam& emitterData);

		//�A�C�e�����T�C�Y�̎擾
		const unsigned int& GetBinSize();

		//���ʃA�C�e���̎擾
		EmitterCommonItem& GetCommonItem();
		
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

	private:
			
		//����J�Ƃ��āA�r���_�[��
		Emitter();

	};

}