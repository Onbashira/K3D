#pragma once
namespace K3D {

	class ParticleItem;

	//エミッタ共通のアイテム2
	struct EmitterHeader {
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

		std::function<void()> _updater;

		std::vector < std::unique_ptr<ParticleItem> >_items;

		unsigned int _dataSize;

	public:

		Emitter();

		~Emitter();

		void UpdateItems();

		void SetCustomUpdater(std::function<void(void)> function);

		const unsigned int& GetBinSize();

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