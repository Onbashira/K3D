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
		//サウンドごとのデータ
		XAUDIO2_BUFFER _audioBuffer;
		//コミット用ソースボイス
		IXAudio2SourceVoice* _sourceVoice;
		//生波形データへの弱参照
		std::weak_ptr<AudioWaveSource> _rawData;
		//コールバック
		AudioCallBack _callBack;
		//音の状態
		XAUDIO2_VOICE_STATE _voiceState;
		//波形データのフォーマット
		WAVEFORMATEXTENSIBLE _format;
		//生データのどの位置から一秒間のサンプリングを行っているかを知らせる要素番号
		unsigned int _seekPoint;
		//曲の最大ながさ。ループポイントを設定するとこの長さが変わる。デフォルトで結尾部分のポイント
		unsigned int _audioLength;
		//ループするかしないか。
		bool _isLoop;
		//破棄されたか
		bool _isDiscarded;

	protected:

	public:

		Audio();

		virtual ~Audio();

		/**
		 * @fn
		 * @brief 再生
		 */
		virtual void Play();

		/**
		 * @fn
		 * @brief 停止
		 */
		virtual void Stop();

		/**
		 * @fn
		 * @brief ループ再生を許可
		 */
		void LoopEnable();
		/**
		 * @fn
		 * @brief ループ再生を不許可
		 */
		void LoopDisable();

		/**
		 * @fn
		 * @brief ループポイントを設定する
		 * @param[in] loopPoint ループ地点
		 */
		void SetLoopPoint(unsigned int loopPoint = 0);

		/**
		 * @fn
		 * @brief 状態の取得
		 */
		XAUDIO2_VOICE_STATE GetState();

		/**
		 * @fn
		 * @brief 一時停止
		 * @param[in] pause trueでポーズ
		 */
		virtual void Pause(bool pause);

		/**
		 * @fn
		 * @brief 再生中かどうか
		 * @return 再生中で真、停止中で偽
		 */
		virtual bool CheckPlaying();

		/**
		 * @fn
		 * @brief 各参照データの所有権の破棄と楽曲停止
		 */
		virtual void Discard();

		/**
		 * @fn
		 * @brief バッファーのサブミット
		 */
		void SubmitBuffer();

	private:

		/**
		 * @fn
		 * @brief 一括コミット
		 */
		void BulkSubmit();

		/**
		 * @fn
		 * @brief ストリーミングコミット
		 */
		void StreamSubmit();

	};
}