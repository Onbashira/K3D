#pragma once

#include <vector>
#include <xaudio2.h>
#include <mmiscapi.h>

namespace K3D {
	class AudioWaveSource
	{
		friend class AudioLoader;

	public:

	protected:
		//!フォーマット
		WAVEFORMATEXTENSIBLE _format;
		//!波形データ
		std::vector<float>	_wave;	//32bit wave

	private:

		struct AudioLoadingInfo {

			//!どれくらい読み込んだか
			unsigned int _loadedSize;

			//!どれくらい読み込みサイクルを行ったか
			unsigned int _cycleNum;

			//!どれくらい読み込みサイクルを行えばいいか
			unsigned int _cycleMax;

			//!どれくらいの配列分読み取ったか
			unsigned int _loadedIndex;

			//!ロード済みフラグ
			bool _isWaveLoaded;

			AudioLoadingInfo() :
				_loadedSize(0), _cycleNum(0), _cycleMax(0), _isWaveLoaded(false), _loadedIndex(0) {};
			~AudioLoadingInfo() {};

		}_loadingInfo;

	public:

		AudioWaveSource();

		virtual ~AudioWaveSource();

		/**
		* @fn
		* @brief フォーマットの取得
		* @return フォーマットへの参照
		*/
		WAVEFORMATEX& GetWaveFormat();

		/**
		* @fn
		* @brief Exフォーマットの取得
		* @return Exフォーマットへの参照
		*/
		WAVEFORMATEXTENSIBLE& GetWaveFormatExtensible();

		/**
		* @fn
		* @brief 波形の取得
		* @return 波形の配列
		*/
		std::vector<float>&	GetWave();

		/**
		* @fn
		* @brief ロードが終了しているか否か
		* @return trueでロード終了、それ以外でロード中
		*/
		bool IsWaveLoaded();

	protected:

	private:

		friend void LoadStereo(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadMonaural(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadStereo8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadStereo16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadStereo24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadStereo32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadMonaural8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadMonaural16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadMonaural24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);

		friend void LoadMonaural32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int rawFormatSampleBit, unsigned int firstReadIndex);
	};
}

struct Stereo8Bit {
	unsigned char left;
	unsigned char right;
};

struct Stereo16Bit {
	short left;
	short right;
};

struct Mono8Bit {
	unsigned char wave;
};

struct Mono16Bit {
	short wave;
};