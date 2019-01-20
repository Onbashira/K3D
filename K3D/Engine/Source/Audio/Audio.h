#pragma once

#include <xaudio2.h>
#include <memory>
#include <array>
#include <deque>
#include "AudioCallBack.h"

namespace K3D {

	class SubMixVoice;
	class AudioWaveSource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;
	public:

	private:
		//�T�E���h���Ƃ̃f�[�^
		XAUDIO2_BUFFER _audioBuffer;
		//�R�~�b�g�p�\�[�X�{�C�X
		IXAudio2SourceVoice* _sourceVoice;
		//���g�`�f�[�^�ւ̎�Q��
		std::weak_ptr<AudioWaveSource> _rawData;
		//�R�[���o�b�N
		AudioCallBack _callBack;
		//���̏��
		XAUDIO2_VOICE_STATE _voiceState;
		//�g�`�f�[�^�̃t�H�[�}�b�g
		WAVEFORMATEXTENSIBLE _format;
		//���f�[�^�̂ǂ̈ʒu�����b�Ԃ̃T���v�����O���s���Ă��邩��m�点��v�f�ԍ�
		unsigned int _seekPoint;
		//�Ȃ̍ő�Ȃ����B���[�v�|�C���g��ݒ肷��Ƃ��̒������ς��B�f�t�H���g�Ō��������̃|�C���g
		unsigned int _audioLength;
		//���[�v���邩���Ȃ����B
		bool _isLoop;
		//�j�����ꂽ��
		bool _isDiscarded;

	protected:

	public:

		Audio();

		virtual ~Audio();

		/**
		 * @fn
		 * @brief �Đ�
		 */
		virtual void Play();

		/**
		 * @fn
		 * @brief ��~
		 */
		virtual void Stop();

		/**
		 * @fn
		 * @brief ���[�v�Đ�������
		 */
		void LoopEnable();
		/**
		 * @fn
		 * @brief ���[�v�Đ���s����
		 */
		void LoopDisable();

		/**
		 * @fn
		 * @brief ���[�v�|�C���g��ݒ肷��
		 * @param[in] loopPoint ���[�v�n�_
		 */
		void SetLoopPoint(unsigned int loopPoint = 0);

		/**
		 * @fn
		 * @brief ��Ԃ̎擾
		 */
		XAUDIO2_VOICE_STATE GetState();

		/**
		 * @fn
		 * @brief �ꎞ��~
		 * @param[in] pause true�Ń|�[�Y
		 */
		virtual void Pause(bool pause);

		/**
		 * @fn
		 * @brief �Đ������ǂ���
		 * @return �Đ����Ő^�A��~���ŋU
		 */
		virtual bool CheckPlaying();

		/**
		 * @fn
		 * @brief �e�Q�ƃf�[�^�̏��L���̔j���Ɗy�Ȓ�~
		 */
		virtual void Discard();

		/**
		 * @fn
		 * @brief �o�b�t�@�[�̃T�u�~�b�g
		 */
		void SubmitBuffer();

	private:

		/**
		 * @fn
		 * @brief �ꊇ�R�~�b�g
		 */
		void BulkSubmit();

		/**
		 * @fn
		 * @brief �X�g���[�~���O�R�~�b�g
		 */
		void StreamSubmit();

	};
}