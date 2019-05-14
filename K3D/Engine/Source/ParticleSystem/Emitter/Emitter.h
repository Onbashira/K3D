#pragma once
namespace K3D {

	class ParticleItem;

	//エミッタ共通のアイテム(CPU側）
	struct CPUEmitterHeader {
		//エミッタ領域のヘッドアドレス
		unsigned int EmitterBinHead = 0;
		//エミッタ領域のサイズ
		unsigned int EmmiterBinSize = 0;
		//パーティクルバイナリのヘッダアドレス
		unsigned int ParticleBinHead = 0;
		//パーティクルバイナリのサイズ
		unsigned int ParticleBinSize = 0;
		//パーティクルインデックス保持領域のインデックス番号
		unsigned int ParticleIdxHead = 0;
		//パーティクルのサイズ
		unsigned int ParticleSize = 0;

		CPUEmitterHeader() : EmitterBinHead(0), EmmiterBinSize(0),
			ParticleBinHead(0), ParticleIdxHead(0), ParticleSize(0) {};

		~CPUEmitterHeader() {};
	};

	struct GPUEmitterHeader {
		//エミッタの領域のヘッドアドレス
		unsigned int EmitterBinHead = 0;
		//パーティクルバイナリのヘッダアドレス
		unsigned int ParticleBinHead = 0;
		//パーティクルインデックス保持領域のインデックス番号
		unsigned int ParticleIdxHead = 0;
		//パーティクルのサイズ
		unsigned int ParticleSize = 0;

		GPUEmitterHeader() : EmitterBinHead(),
			ParticleBinHead(), ParticleIdxHead(), ParticleSize() {};
		GPUEmitterHeader() : EmitterBinHead(),
			ParticleBinHead(), ParticleIdxHead(), ParticleSize() {};
		~GPUEmitterHeader() {};
	};

	//エミッタデータ（GPU内でのみ読み書きされるメモリのフォーマット
	struct EmitterParam {
		//メモリ位置
		unsigned int SpawnHead = 0;
		//スポーン最大数
		unsigned int SpawnMax = 0;
		//パーティクル数
		unsigned int ParticleNum = 0;
	};

	class Emitter
	{
	public:

	private:

		//アイテムアップデータ
		std::function<void()> _updater;

		//アイテム配列
		std::vector < std::unique_ptr<ParticleItem> >_items;

		//エミッタアイテム部のサイズ（Byte)
		unsigned int _dataSize;

		EmitterParam _emitterData;

	public:

		Emitter();

		~Emitter();

		//各アイテムのアップデート
		void UpdateItems();

		//アイテムアップデータの変更
		void SetCustomUpdater(std::function<void(void)> function);

		//エミッタの情報のセット
		void SetEmitterParam(const EmitterParam& emitterData);

		//アイテム部サイズの取得
		const unsigned int& GetBinSize();

		//エミッタアイテム配列の取得
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