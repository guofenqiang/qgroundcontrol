#include "telecontrol.h"
#include "QGCApplication.h"

Telecontrol::Telecontrol(QObject *parent)
    : QObject{parent},
    m_serial(new QSerialPort)
{
#ifdef __android__
    connect(m_serial, &QSerialPort::readyRead, this, &Telecontrol::readData);
    openSerialPort();
    _JoystickTimeUpdater.setInterval(20); // 50Hz, same as real joystick rate
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
    m_serial->setBaudRate(460800);
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
//    const QByteArray data = m_serial->readAll();
//    allAdc(data.data());
    read();
}

void Telecontrol::allAdc(QString ss)
{
    int pos = 0;
    static int state = 0;
    QRegExp re_start("CH0");
    QRegExp re_end("\\r\\n\\r\\n");

    static QStringList strList = {};
    if (state == 0) {
        strList.clear();
        if ((pos = re_start.indexIn(ss)) != -1) {
            ss.remove(0, pos);
            state = 1;
        }
    }
    if (state == 1) {
        strList <<ss;
        if (re_end.indexIn(ss) != -1){
            state = 0;
            regexpNumber(strList);
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
    quint16 flight_mode0 = numList.at(LEFT_BUTTON0);

    dataRoll(roll);
    dataPitch(pitch);
    dataYaw(yaw);
    dataThrust(thrust);

//    qDebug("_roll:%f _pitch: %f, _yaw: %f, _thrust: %f", _roll, _pitch, _yaw, _thrust);
//    qDebug("flight_mode0: %d, armed0: %d\n", flight_mode0, armed0);
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        set_armed(armed0);
        set_flight_mode(flight_mode0);
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

void Telecontrol::set_armed(quint16 num0)
{
    static int last_state = -1;
    static int state = -1;
    /* 右拨钮 */
    if (num0 < (TELE_TOGGLE_BUTTON_TOLERATE)) {
        //0 0   middle
        state = 0;
    } else if((num0 < (TELE_TOGGLE_BUTTON_LOW + TELE_TOGGLE_BUTTON_TOLERATE)) && (num0 > (TELE_TOGGLE_BUTTON_LOW - TELE_TOGGLE_BUTTON_TOLERATE))){
        //0 1   left
//        state = 1;
    } else if (num0 >= (TELE_TOGGLE_BUTTON_HIGH - TELE_TOGGLE_BUTTON_TOLERATE)) {
        //1 0   right
        state = 2;
    }
    if (last_state != state) {
        if (last_state == 0 && state == 2)
        {
            // armed
            qDebug("armed");
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(true, false);
            // Wait 1500 msecs for vehicle to arm (waiting for the next heartbeat)
            for (int i = 0; i < 15; i++) {
                if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->armed()) {
                    break;
                }
                QGC::SLEEP::msleep(100);
                qgcApp()->processEvents(QEventLoop::ExcludeUserInputEvents);
            }

        } else if(last_state == 2 && state == 0) {
            // disarmed
            qDebug("disarmed");
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(false, false);
            // Wait 1500 msecs for vehicle to arm (waiting for the next heartbeat)
            for (int i = 0; i < 15; i++) {
                if (!qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->armed()) {
                    break;
                }
                QGC::SLEEP::msleep(100);
                qgcApp()->processEvents(QEventLoop::ExcludeUserInputEvents);
            }
        } else {
            qDebug("last_state: %d, state: %d, num: %d\n",last_state, state, num0);
        }
    }

    last_state = state;
}

void Telecontrol::set_flight_mode(quint16 num0)
{
    static int last_state = 0;
    static int state = 0;

    /* 左拨钮 */
    if (num0 < (TELE_TOGGLE_BUTTON_TOLERATE)) {
        // 0 0 middle
        state = 0;
    } else if ((num0 < (TELE_TOGGLE_BUTTON_LOW + TELE_TOGGLE_BUTTON_TOLERATE)) && (num0 > (TELE_TOGGLE_BUTTON_LOW - TELE_TOGGLE_BUTTON_TOLERATE))) {
        // 0 1 right
        state = 1;
    } else if (num0 >= (TELE_TOGGLE_BUTTON_HIGH - TELE_TOGGLE_BUTTON_TOLERATE)) {
        // 1 0 left
        state = 2;
    }

    if (last_state != state)
    {
        if (state == 0)
        {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Altitude");
        } else if (state == 1) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Mission");
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

void Telecontrol::read()
{
#define READ_BUFFER_SIZE 300
    // 获取当前缓冲区中的所有可用字节
    while (m_serial->bytesAvailable())
    {
        char buffer[READ_BUFFER_SIZE]; // 定义一个足够大的缓冲区来接收数据
        qint64 len = m_serial->read(buffer, READ_BUFFER_SIZE); // 读取实际的数据量

        // 将新读取的数据添加到整体缓冲区或解析器
        if (len > 0)
        {
            // 根据你的数据包协议解析数据
            parseBuffer(buffer, len);
        }
    }
}

// 假设你有一个基于某种协议的解析函数
void Telecontrol::parseBuffer(const char* buffer, qint64 length)
{
#define PACKET_DELIMITER_LENGTH 4
    static QByteArray accumulatedData; // 用于暂存未完成的数据包

    // 将新读取的数据追加到累积数据中
    accumulatedData.append(buffer, length);

    while (true)
    {
        // 检查是否有完整的数据包（比如检查是否包含包尾）
        int packetEndIndex = findPacketDelimiter(accumulatedData);

        if (packetEndIndex != -1)
        {
            // 找到了完整数据包
            QByteArray packet = accumulatedData.left(packetEndIndex + PACKET_DELIMITER_LENGTH);
            processReceivedPacket(packet); // 处理这个数据包

            // 移除已处理部分，剩余部分留在累计缓冲区
            accumulatedData.remove(0, packetEndIndex + PACKET_DELIMITER_LENGTH);
        }
        else
        {
            // 没有找到完整的数据包，等待更多的数据到来
            break;
        }
    }
}

// 假设你有一个函数来查找数据包的结束位置
int Telecontrol::findPacketDelimiter(const QByteArray& data)
{
    // 这里是根据你的协议来实现的，例如，如果每个包以特定字符序列结束
    const char* delimiter = "\r\n\r\n"; // 仅为示例，实际协议可能不同
    return data.indexOf(delimiter);
}

// 假设你有一个函数来处理解析出的数据包内容
void Telecontrol::processReceivedPacket(const QByteArray& packet)
{
    // 在这里对整个数据包进行解码或进一步处理
    // ...
    //    qDebug() << packet.data();
    regexpNumber(packet);
}

void Telecontrol::regexpNumber(const QByteArray& packet)
{
    QString str = packet.data();
    QStringList strs = findAll(":(\\d+)", str, false);

    // QStringList里的所有字符转换为数字
    QList<quint16> numList;
    foreach(QString s, strs) {
        int num = s.toUShort();
        numList.append(num);
    }
    if (numList.length() < 16) {
        return;
    }
    dataProcess(numList);
}

void Telecontrol::regexpNumber(const QStringList& packet)
{
    QString str = packet.join("");
    QStringList strs = findAll(":(\\d+)", str, false);

    // QStringList里的所有字符转换为数字
    QList<quint16> numList;
    foreach(QString s, strs) {
        int num = s.toUShort();
        numList.append(num);
    }
    if (numList.length() < 16) {
        return;
    }
    dataProcess(numList);
}
