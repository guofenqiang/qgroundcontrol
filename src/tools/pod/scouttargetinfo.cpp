#include "scouttargetinfo.h"

ScoutTargetInfo::ScoutTargetInfo(QObject *parent)
    : QObject{parent}
{

}

ScoutTargetInfo::ScoutTargetInfo(const quint32 timeScale, float azimuth, float freq, float bandWidth, quint32 signalPower, QObject *parent)
    : QObject(parent), _timeScale(timeScale), _azimuth(azimuth), _freq(freq), _bandWidth(bandWidth), _signalPower(signalPower)
{

}

ScoutTargetInfo::~ScoutTargetInfo()
{

}

uint32_t ScoutTargetInfo::timeScale() const
{
    return _timeScale;
}

void ScoutTargetInfo::setTimeScale(quint32 newTimeScale)
{
    if (_timeScale == newTimeScale)
        return;
    _timeScale = newTimeScale;
    emit timeScaleChanged();
}

float ScoutTargetInfo::azimuth() const
{
    return _azimuth;
}

void ScoutTargetInfo::setAzimuth(float newAzimuth)
{
    if (qFuzzyCompare(_azimuth, newAzimuth))
        return;
    _azimuth = newAzimuth;
    emit azimuthChanged();
}

float ScoutTargetInfo::freq() const
{
    return _freq;
}

void ScoutTargetInfo::setFreq(float newFreq)
{
    if (qFuzzyCompare(_freq, newFreq))
        return;
    _freq = newFreq;
    emit freqChanged();
}

float ScoutTargetInfo::bandWidth() const
{
    return _bandWidth;
}

void ScoutTargetInfo::setBandWidth(float newBandWidth)
{
    if (qFuzzyCompare(_bandWidth, newBandWidth))
        return;
    _bandWidth = newBandWidth;
    emit bandWidthChanged();
}

quint32 ScoutTargetInfo::signalPower() const
{
    return _signalPower;
}

void ScoutTargetInfo::setSignalPower(quint32 newSignalPower)
{
    if (_signalPower == newSignalPower)
        return;
    _signalPower = newSignalPower;
    emit signalPowerChanged();
}
