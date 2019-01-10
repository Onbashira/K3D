#pragma once
#include "Audio.h"
#include "Engine/Source/Utility/NonCopyable.h"
#include "Engine/Source/Async/ThreadPool.h"
#include <memory>
#include <string>

namespace K3D {

	class AudioWaveSource;
	class AudioLoader : public NonCopyable
	{

	public:

	private:

		//スレッド数
		static inline unsigned int _THREAD_NUM = 8;
		//ワーカースレッド
		ThreadPool _loadThreadPool;
		//ロード停止フラグ
		bool _isStop;

	public:

		~AudioLoader() {};

		static AudioLoader& GetInstance() {
			static AudioLoader instance;
			return instance;
		}


		void StopLoad();

		void ReStartLoad();

		bool IsStopLoad();

		void DiscardWorkerThreads();

		std::shared_ptr<AudioWaveSource> LoadAudio(std::string audioFilePath);

		std::shared_ptr<AudioWaveSource> LoadAudioEx(std::string audioFilePath);

	private:

		AudioLoader() : _loadThreadPool(_THREAD_NUM) {};

		AudioLoader(const AudioLoader&value) : _loadThreadPool(_THREAD_NUM) {};

		AudioLoader(AudioLoader&&value) : _loadThreadPool(_THREAD_NUM) {};

		void operator = (const AudioLoader& value) {};

		void operator = (AudioLoader&&value) {};
	};


}

