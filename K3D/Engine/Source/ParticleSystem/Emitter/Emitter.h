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
	};

	//エミッタデータ（GPU内でのみ読み書きされるメモリのフォーマット
	struct EmitterData {
		//メモリ位置
		unsigned int SpawnHead = 0;
		//メモリ位置から何個分
		unsigned int SpawnNum = 0;
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

	public:

		Emitter();

		~Emitter();

		//各アイテムのアップデート
		void UpdateItems();

		//アイテムアップデータの変更
		void SetCustomUpdater(std::function<void(void)> function);

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