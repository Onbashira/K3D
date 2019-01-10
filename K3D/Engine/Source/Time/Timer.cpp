#include "Timer.h"
#include <windows.h>



K3D::Timer::Timer() :
	_secondsPerCount(0), _deltaTime(0), _baseTime(0), _pausedTime(0),
	_stopTime(0), _prevTime(0), _currentTime(0), _stopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	_secondsPerCount = 1.0 / (double)countsPerSec;
}


K3D::Timer::~Timer()
{

}

float K3D::Timer::DeltaTime() const
{
	return (float)_deltaTime;
}

float K3D::Timer::TotalTime() const
{
	if (_stopped)
	{
		return (float)(((_stopTime - _pausedTime) - _baseTime)*_secondsPerCount);
	}

	else
	{
		return (float)(((this->_currentTime - _pausedTime) - _baseTime) * _secondsPerCount);
	}
}

void K3D::Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	_baseTime = currTime;
	_prevTime = currTime;
	_stopTime = 0;
	_stopped = false;
}

void K3D::Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (_stopped)
	{
		_pausedTime += (startTime -_stopTime);

		_prevTime = startTime;
		_stopTime = 0;
		_stopped = false;
	}
}

void K3D::Timer::Stop()
{
	if (!_stopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		_stopTime = currTime;
		_stopped = true;
	}
}

void K3D::Timer::Tick()
{
	if (_stopped)
	{
		_deltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	_currentTime = currTime;

	// Time difference between this frame and the previous.
	_deltaTime = (_currentTime - _prevTime)*_secondsPerCount;

	// Prepare for next frame.
	_prevTime = _currentTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (_deltaTime < 0.0)
	{
		_deltaTime = 0.0;
	}
}
