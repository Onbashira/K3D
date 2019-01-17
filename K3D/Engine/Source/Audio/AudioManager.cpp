#include "AudioManager.h"
#include "AudioLoader.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"
#include "Audio.h"
#include "Engine/Source/Utility/Utility.h"
#include "Engine/Source/Debug/Logger.h"

K3D::AudioManager::AudioManager()
{


}

K3D::AudioManager::~AudioManager()
{
	Discard();
}



void K3D::AudioManager::InitializeXAudio2()
{
	if (FAILED(XAudio2Create(&GetInstance()._xAudio2))) {
		ERROR_LOG(std::string("XAudio2�̍쐬�Ɏ��s���܂���"));
	};
	INFO_LOG(std::string("XAudio2�̍쐬�ɐ������܂���"));

	if (FAILED(GetInstance()._xAudio2->CreateMasteringVoice(&GetInstance()._masterVoice))) {
		ERROR_LOG(std::string("MasteringVoice�̍쐬�Ɏ��s���܂���"));
	};
	INFO_LOG(std::string("MasteringVoice�̍쐬�ɐ������܂���"));

}

std::unique_ptr<K3D::Audio> K3D::AudioManager::CreateSourceVoice(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{
	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//�ꊇ����
	audio->_rawData = waveResource;
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());

	audio->BulkSubmit();
	return audio;
}

std::unique_ptr<K3D::Audio> K3D::AudioManager::CreateSourceVoiceEx(std::weak_ptr<AudioWaveSource> waveResource, AudioCallBack * callback, const XAUDIO2_VOICE_SENDS * sendList, const XAUDIO2_EFFECT_CHAIN * effectChain)
{

	std::unique_ptr<Audio> audio = std::make_unique<Audio>();

	if (callback != nullptr) {
		audio->_callBack = *callback;
	}

	_xAudio2->CreateSourceVoice(&audio->_sourceVoice, &waveResource.lock()->GetWaveFormat(), 0, 2.0f, &audio->_callBack, sendList, effectChain);

	//���̃f�[�^�̎Q�Ƃ���������
	audio->_rawData = waveResource;
	//�I�[�ʒu(�ŏI�v�f�ԍ�+1)���� 
	audio->_audioLength = static_cast<unsigned int>(waveResource.lock()->GetWave().size());

	//�P�b���̃f�[�^�̏������f�[�^���̃o�b�t�@���w�肳���� ��b�ԂɕK�v�ȃT���v�����O��*�`�����l����
	unsigned int seekValue = waveResource.lock()->GetWaveFormat().nSamplesPerSec * waveResource.lock()->GetWaveFormat().nChannels;
	unsigned int audioBytePerSec = waveResource.lock()->GetWaveFormat().nAvgBytesPerSec;
	//��b���̃f�[�^���{�L���[�ɑ���
	for (unsigned int i = 0; i < audio->_callBack.AUDIO_BUFFER_QUEUE_MAX; i++) {

		//�������ȃf�[�^���V�[�N�|�C���g + 1�b�Ԃ̃f�[�^�ʂ���b�����Ȃ�
		if (audio->_seekPoint + seekValue >= audio->_audioLength) {

			audio->_audioBuffer.AudioBytes = static_cast<UINT32>((audio->_audioLength - audio->_seekPoint) * sizeof(float));
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			//std::stringstream ss;
			//ss << static_cast<float>((float)audio->_seekPoint / (float)audio->_audioLength) * 100.0f << " % ";
			//DETAILS_LOG(ss.str());

			audio->SubmitBuffer();
			audio->_seekPoint = 0;
			break;
		}
		else {
			//��b�Ԃ̍Đ��ɕK�v�ȃo�C�g��
			audio->_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
			//��b�Ԃ̍Đ����ɕK�v�Ȕg�`�f�[�^�ւ̃|�C���^
			audio->_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&waveResource.lock()->GetWave()[audio->_seekPoint]);

			//std::stringstream ss;
			//ss << static_cast<float>((float)audio->_seekPoint / (float)audio->_audioLength) * 100.0f << " % ";
			//DETAILS_LOG(ss.str());

			audio->SubmitBuffer();
		}
		audio->_seekPoint += seekValue;
	}
	audio->StreamSubmit();
	//�T�u�~�b�g
	return std::move(audio);
}

void K3D::AudioManager::Discard()
{
	AudioLoader::GetInstance().StopLoad();
	AudioLoader::GetInstance().DiscardWorkerThreads();

	if (_masterVoice != nullptr) {
		_masterVoice->DestroyVoice();
		_masterVoice = nullptr;
		INFO_LOG(std::string("MasteringVoice���폜���܂���"));
	}
	if (_xAudio2 != nullptr) {
		_xAudio2->Release();
		_xAudio2 = nullptr;
		INFO_LOG(std::string("XAudio2���폜���܂���"));
	}
}

void K3D::AudioManager::StartSoundEngine()
{
	_xAudio2->StartEngine();
}

void K3D::AudioManager::StopSoundEngine()
{
	_xAudio2->StopEngine();
}

std::unique_ptr<K3D::Audio> K3D::AudioManager::LoadAudio(std::string audioPath)
{
	auto audioResource = AudioLoader::GetInstance().LoadAudioEx(audioPath);

	auto audio = std::move(this->CreateSourceVoiceEx(audioResource));

	return std::move(audio);;
}
