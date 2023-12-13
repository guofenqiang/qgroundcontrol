#ifndef TELECONTROL_H
#define TELECONTROL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Telecontrol : public QObject
{
    Q_OBJECT
public:
    explicit Telecontrol(QObject *parent = nullptr);
    ~Telecontrol();

    enum ModeType {
        LEFT_BUTTON0 = 0, // FLIGHT_MODE
        LEFT_BUTTON1 = 1, // FLIGHT_MODE
        LEFT_POTENTIOMETER,
        ROLL,
        PITCH,
        BUTTON2,
        THRUST,
        YAW,
        RIGHT_BUTTON0, // ARMED_DISARMED
        RIGHT_BUTTON1, // ARMED_DISARMED
        RIGHT_POTENTIOMETER,
    };

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();

signals:

private:
    QSerialPort *m_serial = nullptr;
    QStringList findAll(QString pattern,QString str,bool noGreedy);
    void allAdc(QString ss);
    void dataProcess(QList<quint16> numList);
    void dataRoll(quint16 num);
    void dataPitch(quint16 num);
    void dataThrust(quint16 num);
    void dataYaw(quint16 num);
    void set_armed(quint16 num0, quint16 num1);
    void set_flight_mode(quint16 num0, quint16 num1);
};

#endif // TELECONTROL_H
