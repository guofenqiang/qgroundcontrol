#ifndef TELECONTROL_H
#define TELECONTROL_H

#include <QTimer>
#include <QObject>
#include <QtSerialPort/QSerialPort>

#define TELE_ADC_NUM					16
#define TELE_ADC_BUFF                   5

class Telecontrol : public QObject
{
    Q_OBJECT
public:
    explicit Telecontrol(QObject *parent = nullptr);
    ~Telecontrol();

    enum ModeType {
/*        左拨钮0
 *        左拨钮1
 *        左电位器
 *        左摇杆X
 *        左摇杆Y
 *        按键 3个AD值（0：无， 1：左，2：中，3：右）
 *        右摇杆X
 *        右摇杆Y
 *        右电位器
 *        右拨钮0
 *        右拨钮1
 */
        RIGHT_BUTTON0 = 0, // ARMED_DISARMED
        RIGHT_BUTTON1, // ARMED_DISARMED
        RIGHT_POTENTIOMETER, //右电位器，顺时针值变小，逆时针值变大
        RIGHT_X, // 右摇杆X, 向左值变大，向右值变小
        RIGHT_Y, // 右摇杆Y，向上值变大, 向下值变小
        BUTTON2,
        LEFT_X, // 左摇杆X, 向左值变大，向右值变小
        LEFT_Y, // 左摇杆Y，向上值变大, 向下值变小
        LEFT_BUTTON0, // FLIGHT_MODE
        LEFT_BUTTON1, // FLIGHT_MODE
        LEFT_POTENTIOMETER, //左电位器，顺时针值变大，逆时针值变小
    };

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void _updateJoystickTime();

signals:

private:
    QSerialPort *m_serial = nullptr;
    QTimer      _JoystickTimeUpdater;
    float _roll;
    float _pitch;
    float _yaw;
    float _thrust;
    quint16 _static_err_buff[TELE_ADC_NUM][TELE_ADC_BUFF];
    float _static_err[TELE_ADC_NUM];
    QStringList findAll(QString pattern,QString str,bool noGreedy);
    void allAdc(QString ss);
    void dataProcess(QList<quint16> numList);
    void dataRoll(float num);
    void dataPitch(float num);
    void dataThrust(float num);
    void dataYaw(float num);
    void set_armed(quint16 num0, quint16 num1);
    void set_flight_mode(quint16 num0, quint16 num1);
    void _CalculateStaticError(QList<quint16> numList);
};

#endif // TELECONTROL_H
