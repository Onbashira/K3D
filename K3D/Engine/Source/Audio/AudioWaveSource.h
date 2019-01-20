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
		//!�t�H�[�}�b�g
		WAVEFORMATEXTENSIBLE _format;
		//!�g�`�f�[�^
		std::vector<float>	_wave;	//32bit wave

	private:

		struct AudioLoadingInfo {

			//!�ǂꂭ�炢�ǂݍ��񂾂�
			unsigned int _loadedSize;

			//!�ǂꂭ�炢�ǂݍ��݃T�C�N�����s������
			unsigned int _cycleNum;

			//!�ǂꂭ�炢�ǂݍ��݃T�C�N�����s���΂�����
			unsigned int _cycleMax;

			//!�ǂꂭ�炢�̔z�񕪓ǂݎ������
			unsigned int _loadedIndex;

			//!���[�h�ς݃t���O
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
		* @brief �t�H�[�}�b�g�̎擾
		* @return �t�H�[�}�b�g�ւ̎Q��
		*/
		WAVEFORMATEX& GetWaveFormat();

		/**
		* @fn
		* @brief Ex�t�H�[�}�b�g�̎擾
		* @return Ex�t�H�[�}�b�g�ւ̎Q��
		*/
		WAVEFORMATEXTENSIBLE& GetWaveFormatExtensible();

		/**
		* @fn
		* @brief �g�`�̎擾
		* @return �g�`�̔z��
		*/
		std::vector<float>&	GetWave();

		/**
		* @fn
		* @brief ���[�h���I�����Ă��邩�ۂ�
		* @return true�Ń��[�h�I���A����ȊO�Ń��[�h��
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