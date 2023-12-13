#ifndef TELECONTROL_H
#define TELECONTROL_H

#include <QObject>
#ifdef __android__
#include "qserialport.h"
#else
#include <QSerialPort>
#endif

class Telecontrol : public QObject
{
    Q_OBJECT
public:
    explicit Telecontrol(QObject *parent = nullptr);
    ~Telecontrol();
    QStringList findAll(QString pattern,QString str,bool noGreedy);
    void allAdc(QString ss);
    void dataProcess(QList<quint16> numList);
    void dataRoll(quint16 num);
    void dataPitch(quint16 num);
    void dataThrust(quint16 num);
    void dataYaw(quint16 num);

    enum ModeType {
        ROLL = 0,
        PITCH,
        THRUST,
        YAW,
        ARMED_DISARMED,
        FLIGHT_MODE,
    };

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();

signals:

private:
    QSerialPort *m_serial = nullptr;
};

#endif // TELECONTROL_H
