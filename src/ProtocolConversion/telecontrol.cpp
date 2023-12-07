#include "telecontrol.h"
#include "QGCApplication.h"

Telecontrol::Telecontrol(QObject *parent)
    : QObject{parent},
    m_serial(new QSerialPort)
{
    connect(m_serial, &QSerialPort::readyRead, this, &Telecontrol::readData);
    openSerialPort();
}

Telecontrol::~Telecontrol()
{
    closeSerialPort();
}

void Telecontrol::openSerialPort()
{
    m_serial->setPortName("COM8");
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        qDebug("Connected to %s\n", "COM8");
    } else {
        qDebug() << "Open error";
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
            qDebug() << numList;
            dataProcess(numList);
        }
    }
}

QStringList Telecontrol::findAll(QString pattern,QString str,bool noGreedy){
    //    QRegExp rxlen("u001B\[[0-9]+;[0-9]+m(.*)[ ]+");//构造 QRegExp 对象
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
    double roll = -((static_cast<double>(numList.at(ROLL)) / 2048) - 1);
    double pitch = (static_cast<double>(numList.at(PITCH)) / 2048) - 1;
    double thrust = (static_cast<double>(numList.at(THRUST)) / 4096);
    double yaw = (static_cast<double>(numList.at(YAW)) / 2048) - 1;
    quint16 armed = numList.at(ROLL);
    quint16 flight_mode = numList.at(PITCH);

    qDebug("roll:%f pitch: %f, yaw: %f, thrust: %f\n", roll, pitch, yaw, thrust);
    if (qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
//        qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->virtualTabletJoystickValue(roll, pitch, yaw, thrust);

        if (armed < 2048) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(false, false);
        } else {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setArmed(true, false);
        }

        if (flight_mode < 1360) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Stabilized");
        } else if (flight_mode >= 1360 && flight_mode < 2720) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Altitude");
        } else if (flight_mode >= 2720 && flight_mode < 4096) {
            qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->setFlightMode("Position");
        }
    }
}


void Telecontrol::dataRoll(quint16 num)
{

}
void Telecontrol::dataPitch(quint16 num)
{

}
void Telecontrol::dataThrust(quint16 num)
{

}
void Telecontrol::dataYaw(quint16 num)
{

}
