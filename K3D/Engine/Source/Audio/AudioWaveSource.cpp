#include "AudioWaveSource.h"

K3D::AudioWaveSource::AudioWaveSource() :
	_loadingInfo({}), _format({})
{
}

K3D::AudioWaveSource::~AudioWaveSource()
{
	this->_wave.clear();
	this->_wave.shrink_to_fit();
}


WAVEFORMATEX & K3D::AudioWaveSource::GetWaveFormat()
{
	return _format.Format;
}

WAVEFORMATEXTENSIBLE & K3D::AudioWaveSource::GetWaveFormatExtensible()
{
	return _format;
}

std::vector<float>& K3D::AudioWaveSource::GetWave()
{
	return _wave;
}

bool K3D::AudioWaveSource::IsWaveLoaded()
{
	return _loadingInfo._isWaveLoaded;
}
