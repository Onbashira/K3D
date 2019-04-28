#include "AudioCallBack.h"



K3D::AudioCallBack::AudioCallBack() :
	 _onVoiceProcessingPassStart([](UINT32) {})
	, _onVoiceProcessingPassEnd([]() {}), _onStreamEnd([]() {}), _onBufferStart([](void*) {})
	, _onBufferEnd([](void*) {}), _onLoopEnd([](void*) {}), _onVoiceError([](void*,HRESULT) {})

{

}


K3D::AudioCallBack::~AudioCallBack()
{
}


void K3D::AudioCallBack::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
	_onVoiceProcessingPassStart(BytesRequired);
}

void K3D::AudioCallBack::OnVoiceProcessingPassEnd(void)
{
	_onVoiceProcessingPassEnd();
}

void K3D::AudioCallBack::OnStreamEnd(void)
{
	_onStreamEnd();
}

void K3D::AudioCallBack::OnBufferStart(void * pBufferContext)
{
	_onBufferStart(pBufferContext);
}

void K3D::AudioCallBack::OnBufferEnd(void * pBufferContext)
{
	_onBufferEnd(pBufferContext);
}

void K3D::AudioCallBack::OnLoopEnd(void * pBufferContext)
{
	_onLoopEnd(pBufferContext);
}

void K3D::AudioCallBack::OnVoiceError(void * pBufferContext, HRESULT Error)
{

	_onVoiceError(pBufferContext, Error);
}

void K3D::AudioCallBack::SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func)
{
	_onVoiceProcessingPassStart = func;
}

void K3D::AudioCallBack::SetOnVoiceProcessingPassEnd(std::function<void()> func)
{
	_onVoiceProcessingPassEnd = func;
}

void K3D::AudioCallBack::SetOnStreamEnd(std::function<void()> func)
{
	_onStreamEnd = func;
}

void K3D::AudioCallBack::SetOnBufferStart(std::function<void(void*)> func)
{
	_onBufferStart = func;
}

void K3D::AudioCallBack::SetOnBufferEnd(std::function<void(void*)> func)
{
	_onBufferEnd = func;
}

void K3D::AudioCallBack::SetOnLoopEnd(std::function<void(void*)> func)
{
	_onLoopEnd = func;
}

void K3D::AudioCallBack::SetOnVoiceError(std::function<void(void*, HRESULT)> func)
{
	_onVoiceError = func;
}
