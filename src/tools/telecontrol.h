#ifndef TELECONTROL_H
#define TELECONTROL_H

#include "Vehicle.h"
#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>

#define TELE_ADC_NUM					16
#define TELE_ADC_BUFF                   5

#define TELE_TOGGLE_BUTTON_LOW          800
#define TELE_TOGGLE_BUTTON_HIGH         1630
#define TELE_TOGGLE_BUTTON_TOLERATE     20
#define TELE_TOGGLE_ADC_HIGH            4000

#define THRESHOLD                       10

class Telecontrol : public QObject
{
    Q_OBJECT
public:
    explicit Telecontrol(QObject *parent = nullptr);
    ~Telecontrol();

    enum ModeType {
/*        左拨钮
 *        左电位器
 *        左摇杆X
 *        左摇杆Y
 *        按键 3个AD值（0：无， 1：左，2：中，3：右）
 *        右摇杆X
 *        右摇杆Y
 *        右电位器
 *        右拨钮
 */
        LEFT_BUTTON0 = 0, // FLIGHT_MODE
        LEFT_POTENTIOMETER, //左电位器，顺时针值变大，逆时针值变小
        LEFT_X, // 左摇杆X, 向左值变大，向右值变小
        LEFT_Y, // 左摇杆Y，向上值变大, 向下值变小
        BUTTON2,
        RIGHT_X, // 右摇杆X, 向左值变大，向右值变小
        RIGHT_Y, // 右摇杆Y，向上值变大, 向下值变小
        RIGHT_POTENTIOMETER, //右电位器，顺时针值变小，逆时针值变大
        RIGHT_BUTTON0, // ARMED_DISARMED
    };

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void _updateJoystickTime();

signals:

private:
    int64_t test_count = 0;
    quint16 test_buff[16] = {0};
    QSerialPort *m_serial = nullptr;
    QTimer      _JoystickTimeUpdater;
    float _roll;
    float _pitch;
    float _yaw;
    float _thrust;
    quint16 _static_err_buff[TELE_ADC_NUM][TELE_ADC_BUFF];
    float _static_err[TELE_ADC_NUM];
    QStringList findAll(QString pattern,QString str,bool noGreedy);
    int buff[THRESHOLD] = {-1};
    int consecutiveCount = 0;
    int flightModeBuff[THRESHOLD] = {-1};
    int flightConsecutiveCount = 0;
    void allAdc(QString ss);
    void dataProcess(QList<quint16> numList);
    void dataRoll(float num);
    void dataPitch(float num);
    void dataThrust(float num);
    void dataYaw(float num);
    void set_armed(quint16 num0);
    void set_flight_mode(quint16 num0);
    bool _isFinishCalculateStaticError(QList<quint16> numList);
    void read();
    void parseBuffer(const char* buffer, qint64 length);
    int findPacketDelimiter(const QByteArray& data);
    void processReceivedPacket(const QByteArray& packet);
    void regexpNumber(const QByteArray& packet);
    void regexpNumber(const QStringList& packet);
    void updateBuff(int newData);
    bool detectTransition(int newData);
    void handleNewData(int newData);
    void transitionArmed(int newData);
    void updateArmed(quint16 num);
    void updateFlightMode(quint16 num);
    bool _setFlightModeAndValidate(Vehicle* vehicle, const QString& flightMode);
};

#endif // TELECONTROL_H
