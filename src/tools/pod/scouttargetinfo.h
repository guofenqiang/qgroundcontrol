#ifndef SCOUTTARGETINFO_H
#define SCOUTTARGETINFO_H

#include <QObject>

class ScoutTargetInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint32 timeScale READ timeScale WRITE setTimeScale NOTIFY timeScaleChanged)
    Q_PROPERTY(float azimuth READ azimuth WRITE setAzimuth NOTIFY azimuthChanged)
    Q_PROPERTY(float freq READ freq WRITE setFreq NOTIFY freqChanged)
    Q_PROPERTY(float bandWidth READ bandWidth WRITE setBandWidth NOTIFY bandWidthChanged)
    Q_PROPERTY(quint32 signalPower READ signalPower WRITE setSignalPower NOTIFY signalPowerChanged)

public:
    ScoutTargetInfo(QObject *parent = nullptr);
    ScoutTargetInfo(const quint32 timeScale, float azimuth, float freq, float bandWidth, quint32 signalPower, QObject *parent = nullptr);
    ~ScoutTargetInfo();

    quint32 timeScale() const;
    void setTimeScale(quint32 newTimeScale);

    float azimuth() const;
    void setAzimuth(float newAzimuth);

    float freq() const;
    void setFreq(float newFreq);

    float bandWidth() const;
    void setBandWidth(float newBandWidth);

    quint32 signalPower() const;
    void setSignalPower(quint32 newSignalPower);

signals:
    void signalList(QList<QObject*> data);

    void timeScaleChanged();

    void azimuthChanged();

    void freqChanged();

    void bandWidthChanged();

    void signalPowerChanged();

private:
    quint32     _timeScale;
    float       _azimuth;
    float       _freq;
    float       _bandWidth;
    quint32     _signalPower;

};

#endif // SCOUTTARGETINFO_H
