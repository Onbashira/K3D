
#include <iomanip>
#include <sstream>
#include "Audio.h"
#include "AudioWaveSource.h"
#include "AudioCallBack.h"
#include "AudioWaveSource.h"


K3D::Audio::Audio() :
	_seekPoint(0), _isLoop(false), _isDiscarded(false), _audioLength(0),
	_loopHead(0),_loopTail(0)
{
}

K3D::Audio::~Audio()
{
	Discard();
}


void K3D::Audio::BulkSubmit()
{

	_seekPoint = 0;

	_audioBuffer.AudioBytes = static_cast<UINT32>(_rawData.lock()->GetWaveFormat().nAvgBytesPerSec);
	_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[_loopHead]);
	SubmitBuffer();

	this->_callBack.SetOnBufferEnd([this](void* context) {
		this->DirectBulkSubmit();
	});
}

void K3D::Audio::StreamSubmit()
{
	_callBack.SetOnBufferEnd([this](void* context) {

		this->DirectStreamSubmit();
	});
}

void K3D::Audio::DirectBulkSubmit()
{
	if (_isDiscarded) {
		this->_callBack.SetOnBufferEnd([](void* context) {});
		return;
	}
	_audioBuffer.AudioBytes = static_cast<UINT32>(_rawData.lock()->GetWave().size() * sizeof(float));
	_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[0]);
	_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

	if (_isLoop == false) {
		Stop();
	}
	SubmitBuffer();
}

void K3D::Audio::DirectStreamSubmit()
{
	if (_isDiscarded)
	{
		this->_callBack.SetOnBufferEnd([](void* context) {});
		return;
	}
	auto state = GetState();

	//もしキュー内のバッファがQ設定数値以下ならバッファに対して新しいデータを供給する
	if (state.BuffersQueued < _callBack.AUDIO_BUFFER_QUEUE_MAX)
	{
		// 44.1k * byte * channel
		unsigned int seekValue = _rawData.lock()->GetWaveFormat().nSamplesPerSec * _rawData.lock()->GetWaveFormat().nChannels;
		unsigned int audioBytePerSec = _rawData.lock()->GetWaveFormat().nAvgBytesPerSec;
		unsigned int seekPoint = (_loopHead + _seekPoint);

		if (seekPoint >= _loopTail)
		{
			_seekPoint = 0;
			seekPoint = (_loopHead + _seekPoint);

			_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
			_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[seekPoint]);
			_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;

			SubmitBuffer();
			_seekPoint += seekValue;
			if (_isLoop == false)
			{
				Stop();
			}
			return;
		}


		//もしも曲データがシークポイント + 一秒間のデータ量が一秒以下もしくはループポイントに到達しているなら
		if ((seekPoint + seekValue) >= _loopTail)
		{
			unsigned int  byte = _loopTail - seekPoint;
			//全体のバイト数　－　現在のサンプル点
			_audioBuffer.AudioBytes = static_cast<UINT32>((byte) * sizeof(float));
			_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[seekPoint]);
			_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
			SubmitBuffer();

			_seekPoint += byte;
			return;
		}
		else
		{
			_audioBuffer.AudioBytes = static_cast<UINT32>(audioBytePerSec);
			_audioBuffer.pAudioData = reinterpret_cast<BYTE*>(&_rawData.lock()->GetWave()[seekPoint]);
			_audioBuffer.Flags = XAUDIO2_END_OF_STREAM;
			SubmitBuffer();
			_seekPoint += seekValue;

		}
	}
}

void K3D::Audio::Play()
{
	this->_sourceVoice->Start();
}

void K3D::Audio::Stop()
{
	this->_sourceVoice->Stop();
}

void K3D::Audio::LoopEnable()
{
	_isLoop = true;
}

void K3D::Audio::LoopDisable()
{
	_isLoop = false;
}

void K3D::Audio::SetLoopPoints(float headPointTime, float tailPointTime)
{
	this->_seekPoint = 0;
	SetLoopHeadPoint(headPointTime);
	SetLoopTailPoint(tailPointTime);
	this->_sourceVoice->Stop();
	this->_callBack.SetOnBufferEnd([](void* context) {});

	this->_sourceVoice->FlushSourceBuffers();
	this->DirectStreamSubmit();
	this->StreamSubmit();
}

void K3D::Audio::SetLoopHeadPoint(float headPointTime)
{
	float seekValue = static_cast<float> (_rawData.lock()->GetWaveFormat().nSamplesPerSec * _rawData.lock()->GetWaveFormat().nChannels);
	this->_loopHead = static_cast<unsigned int> (seekValue * headPointTime);
}

void K3D::Audio::SetLoopTailPoint(float tailPointTime)
{
	float seekValue = static_cast<float> (_rawData.lock()->GetWaveFormat().nSamplesPerSec * _rawData.lock()->GetWaveFormat().nChannels);
	this->_loopHead = static_cast<unsigned int> (seekValue * tailPointTime);
}

XAUDIO2_VOICE_STATE K3D::Audio::GetState()
{
	_sourceVoice->GetState(&_voiceState);
	return this->_voiceState;
}

void K3D::Audio::Pause(bool pause)
{
	if (pause) {
		_sourceVoice->Stop();
	}
	else {
		_sourceVoice->Start();
	}
}

void K3D::Audio::SetVolume(float volume)
{
	_sourceVoice->SetVolume(volume);
}

bool K3D::Audio::CheckPlaying()
{
	XAUDIO2_VOICE_STATE state;
	_sourceVoice->GetState(&state);
	return state.BuffersQueued > 0;
}

void K3D::Audio::Discard()
{
	if (this->_sourceVoice != nullptr) {
		Stop();
		this->_callBack.SetOnBufferEnd([](void* context) {});
		_sourceVoice->FlushSourceBuffers();
		_sourceVoice->Discontinuity();
		_sourceVoice->DestroyVoice();
		//ヌルポインタの代入
		_sourceVoice = nullptr;
		//ソースボイスは消していけない・・・？
		//delete _sourceVoice;
	}
	_isDiscarded = true;
	this->_rawData.reset();
}

void K3D::Audio::SubmitBuffer()
{
	this->_sourceVoice->SubmitSourceBuffer(&this->_audioBuffer);
}
