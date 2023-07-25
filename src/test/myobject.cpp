#include "myobject.h"
#include <LinkInterface.h>
#include <Vehicle.h>
#include <VehicleLinkManager.h>
#include "MAVLinkProtocol.h"
//#include "mavlink_msg_radar_cmd.h"

MyObject::MyObject(QObject *parent)
    : QObject{parent}
{

}

MyObject *MyObject::getInstance()
{
    static MyObject * obj = new MyObject();
    return obj;
}

int MyObject::iValue() const
{
    return m_iValue;
}

void MyObject::setIValue(int newIValue)
{
    if (m_iValue == newIValue)
        return;
    m_iValue = newIValue;
    emit iValueChanged();
}

QString MyObject::sString() const
{
    return m_sString;
}

void MyObject::setSString(const QString &newSString)
{
    if (m_sString == newSString)
        return;
    m_sString = newSString;
    emit sStringChanged();
}

void MyObject::func()
{
    emit cppSig(99, "wangwu");
    qDebug() << __FUNCTION__;
}

void MyObject::cppSlot(int i, QString s)
{
    qDebug() << __FUNCTION__ << " " << i << " " << s;

}

void MyObject::_radarCmd(int cmd)
{
//    SharedLinkInterfacePtr sharedLink = VehicleLinkManager()->primaryLink().lock();
//    if (!sharedLink) {
//        qCDebug(VehicleLog) << "_handlePing: primary link gone!";
//        return;
//    }

//    mavlink_message_t msg;
//    const uint8_t parameter[MAVLINK_MSG_RADAR_CMD_FIELD_PARAMETER_LEN] = {0, 0, 0, 0, 0};
//    mavlink_msg_radar_cmd_pack_chan(128,
//                                    0,
//                                    0,
//                                    &msg, cmd, parameter);
//    // Write message into buffer, prepending start sign
//    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
//    int len = mavlink_msg_to_send_buffer(buffer, &msg);

//    sharedLink.get()->writeBytesThreadSafe((const char*)buffer, len);

//    qDebug() << "vehicle: ========= send radar_cmd message ========= ";
//    qDebug() << "cmd:" << cmd;
}

void MyObject::_handleRadarCmdAck(mavlink_message_t& message)
{
//    mavlink_radar_cmd_ack_t ack;
//    mavlink_msg_radar_cmd_ack_decode(&message, &ack);
//    qDebug() << "Vehicle: ========= receive radar_cmd_ack ========= ";
//    qDebug() << "cmd_result:" << ack.cmd_result << " cmd_ack:" << ack.cmd_ack << " parameter:" << ack.parameter;
}

void MyObject::_handleRadarMeasure(mavlink_message_t& message)
{
//    mavlink_radar_measure_t ack;
//    mavlink_msg_radar_measure_decode(&message, &ack);

//    qDebug() << "Vehicle: ========= receive radar_measure ========= ";
//    qDebug() << "time_usec" << ack.time_usec;
//    qDebug() << "frame_number" << ack.frame_number;
//    qDebug() << "pdw1_target_number:" << ack.pdw1_target_number << " pdw1_rate" << ack.pdw1_rate << "pdw1_azimuth" << ack.pdw1_azimuth;
//    qDebug() << "pdw2_target_number:" << ack.pdw2_target_number << " pdw2_rate" << ack.pdw2_rate << "pdw2_azimuth" << ack.pdw2_azimuth;
//    qDebug() << "pdw3_target_number:" << ack.pdw3_target_number << " pdw3_rate" << ack.pdw3_rate << "pdw3_azimuth" << ack.pdw3_azimuth;
//    qDebug() << "pdw4_target_number:" << ack.pdw4_target_number << " pdw4_rate" << ack.pdw4_rate << "pdw4_azimuth" << ack.pdw4_azimuth;
//    qDebug() << "pdw5_target_number:" << ack.pdw5_target_number << " pdw5_rate" << ack.pdw5_rate << "pdw5_azimuth" << ack.pdw5_azimuth;

//    setTime_usec(ack.time_usec);
//    setFrame_number(ack.frame_number);
//    setPdw1_target_number(ack.pdw1_target_number);
//    setPdw2_target_number(ack.pdw2_target_number);
//    setPdw3_target_number(ack.pdw3_target_number);
//    setPdw4_target_number(ack.pdw4_target_number);
//    setPdw5_target_number(ack.pdw5_target_number);

//    setPdw1_rate(ack.pdw1_rate);
//    setPdw2_rate(ack.pdw2_rate);
//    setPdw3_rate(ack.pdw3_rate);
//    setPdw4_rate(ack.pdw4_rate);
//    setPdw5_rate(ack.pdw5_rate);

//    setPdw1_azimuth(ack.pdw1_azimuth);
//    setPdw2_azimuth(ack.pdw2_azimuth);
//    setPdw3_azimuth(ack.pdw3_azimuth);
//    setPdw4_azimuth(ack.pdw4_azimuth);
//    setPdw5_azimuth(ack.pdw5_azimuth);
}
