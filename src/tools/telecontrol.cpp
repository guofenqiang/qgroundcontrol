#include "telecontrol.h"
#include "QGCApplication.h"

Telecontrol::Telecontrol(QObject *parent)
    : QObject{parent},
    m_serial(new QSerialPort)
{
#ifdef __android__
    connect(m_serial, &QSerialPort::readyRead, this, &Telecontrol::readData);
    openSerialPort();
    _JoystickTimeUpdater.setInterval(40); // 25Hz, same as real joystick rate
    connect(&_JoystickTimeUpdater, &QTimer::timeout, this, &Telecontrol::_updateJoystickTime);
    _JoystickTimeUpdater.start();
#endif
}

Telecontrol::~Telecontrol()
{
    closeSerialPort();
}

void Telecontrol::openSerialPort()
{
    m_serial->setPortName("ttyUSB0");
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug("Connected to %s\n", "ttyUSB0");
    } else {
        qDebug() << "Open uart error";
    }
}

void Telecontrol::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void Telecontrol::writeData(const QByteArray &data)
{
    m_serial->write(data);
}

void Telecontrol::readData()
{
    const QByteArray data = m_serial->readAll();
    allAdc(data.data());
}

void Telecontrol::allAdc(QString ss)
{
    static int count = 0;
    QRegExp re_start("CH0");
    QRegExp re_end("(\\r\\n\\r\\n)");

    static QStringList strList = {};
    if (count == 0) {
        count++;
        strList.clear();
        if (re_start.indexIn(ss) != -1) {
            strList << ss;
        }
    } else {
        count++;
        strList <<ss;
        if (re_end.indexIn(ss) != -1){
            count = 0;
            QString str = strList.join("");
            QStringList strs = findAll(":(\\d+)", str, false);

            // QStringList里的所有字符转换为数字
            QList<quint16> numList;
            foreach(QString s, strs) {
                int num = s.toUShort();
                numList.append(num);
            }
//            qDebug() << numList;
            dataProcess(numList);
        }
    }
}

QStringList Telecontrol::findAll(QString pattern,QString str,bool noGreedy){
    QRegExp rx(pattern);//构造 QRegExp 对象
    rx.setMinimal(noGreedy);
    int pos = 0;  //匹配的位置
    QStringList strList;
    while ((pos = rx.indexIn(str, pos)) != -1) {
        strList << rx.cap(1);//得到匹配的字符串,,cap(index),index = 0是整个字符串;当 index >= 1 得到的是 QRegExp 中小括号中匹配的字符串
        pos += rx.matchedLength(); // 刷新匹配的位置
    }
    return strList;
}

void Telecontrol::dataProcess(QList<quint16> numList)
{
    /*
    roll: [-1, 1]
    pitch: [-1, 1]
    yaw: [-1, 1]
    thrust: [0, 1]
*/
    //启动后计算出一个静态误差
    _CalculateStaticError(numList);

    float roll = ((static_cast<float>(numList.at(RIGHT_X)) / 2048) - 1);
    float pitch = ((static_cast<float>(numList.at(RIGHT_Y)) / 2048) - 1);
    float yaw = ((static_cast<float>(numList.at(LEFT_X)) / 2048) - 1);
    float thrust = (static_cast<float>(numList.at(LEFT_Y)) / 4096);
    quint16 armed0 = numList.at(RIGHT_BUTTON0);
    quint16 armed1 = numList.at(RIGHT_BUTTON1);
    quint16 flight_mode0 = numList.at(LEFT_BUTTON0);
    quint16 flight_mode1 = numList.at(LEFT_BUTTON1);

    dataRoll(roll);
    dataPitch(pitch);
    dataYaw(yaw);
    dataThrust(thrust);

//    qDebug("roll:%f pitch: %f, yaw: %f, thrust: %f", roll, pitch, yaw, thrust);
//    qDebug("armed0: %d, armed1: %d, flight_mode0: %d, flight_mode1: %d\n", armed0, armed1, flight_mode0, flight_mode1);
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        set_armed(armed0, armed1);
        set_flight_mode(flight_mode0, flight_mode1);
    }
}


void Telecontrol::dataRoll(float num)
{
    _roll = -round((num - (_static_err[RIGHT_X] - 1)) * 100) / 100;
}
void Telecontrol::dataPitch(float num)
{
    _pitch = round((num - (_static_err[RIGHT_Y] - 1)) * 100) / 100;
}
void Telecontrol::dataYaw(float num)
{
    _yaw = -round((num - (_static_err[LEFT_X] - 1)) * 100) / 100;
}
void Telecontrol::dataThrust(float num)
{
    _thrust = round((num - (_static_err[LEFT_Y] - 1)) * 100) / 100;
}

void Telecontrol::set_armed(quint16 num0, quint16 num1)
{
    static int last_state = 0;
    static int state = 0;
    /* 右拨钮 */
    if (num0 < 3000 && num1 < 3000) {
        //0 0   middle
        state = 0;
    } else if(num0 >= 3000 && num1 < 3000){
        //0 1   left
        state = 1;
    } else if (num0 < 3000 && num1 >= 3000) {
        //1 0   right
        state = 2;
    }
    if (last_state == 0 && state == 2)
    {
        // armed
        qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(true, false);
    } else if(last_state == 2 && state == 0) {
        // disarmed
        qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(false, false);
    }
    last_state = state;
}

void Telecontrol::set_flight_mode(quint16 num0, quint16 num1)
{
    static int last_state = 0;
    static int state = 0;

    /* 左拨钮 */
    if (num0 < 3000 && num1 < 3000) {
        // 0 0 middle
        state = 0;
    } else if (num0 >= 3000 && num1 < 3000) {
        // 0 1 right
        state = 1;
    } else if (num0 < 3000 && num1 >= 3000) {
        // 1 0 left
        state = 2;
    }

    if (last_state != state)
    {
        if (state == 0)
        {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Altitude");
        } else if (state == 1) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Stabilized");
        } else if (state == 2) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Position");
        }

    }
    last_state = state;
}

void Telecontrol::_updateJoystickTime()
{
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
//        qDebug("_roll:%f pitch: %f, yaw: %f, thrust: %f", _roll, _pitch, _yaw, _thrust);
        qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->sendJoystickDataThreadSafe(_roll, _pitch, _yaw, _thrust, 0);
    }
}

void Telecontrol::_CalculateStaticError(QList<quint16> numList)
{
    static int count = 0;
    if (count == TELE_ADC_BUFF) {
        for (int j = 0; j < TELE_ADC_NUM; j++)
        {
            // 除去头上的第一个数据
            for(int k = 1; k < TELE_ADC_BUFF; k++){
                _static_err[j] += _static_err_buff[j][k];
            }
            _static_err[j] = (_static_err[j] / (TELE_ADC_BUFF - 1) / 2048);
            qDebug("static_err: %d, %f", j, _static_err[j]);
        }
    } else if(count > TELE_ADC_BUFF) {
        return;
    }

    for (int i = 0; i < TELE_ADC_NUM; i++)
    {
        _static_err_buff[i][count] = numList.at(i);
    }
    count++;

};
