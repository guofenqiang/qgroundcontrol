#include "ecmcontrol.h"
#include "QGCApplication.h"

ECMControl::ECMControl(QObject *parent)
    : QObject{parent}
{
    _initSignle();
}

void ECMControl::_initSignle()
{
    connect(qgcApp()->toolbox()->mavlinkProtocol(), &MAVLinkProtocol::messageReceived, this, &ECMControl::_mavlinkMessageReceived);
}

void ECMControl::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
{
    Vehicle *_vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (_vehicle == NULL) {
        return;
    }

    Q_UNUSED(link);

    if (message.sysid != _vehicle->id()) {
        return;
    }

    switch (message.msgid) {
    case MAVLINK_MSG_ID_TUNNEL:
        _handleTunnel(message);
        break;
    }
}

void ECMControl::_handleTunnel(mavlink_message_t message)
{
    mavlink_tunnel_t tunnel;

    mavlink_msg_tunnel_decode(&message, &tunnel);

    QByteArray byte((char*)tunnel.payload, tunnel.payload_length);
    qDebug() << byte.toHex(' ');
}

void ECMControl::StandbyCmd()
{
    qDebug() << "StandbyCmd";
}

void ECMControl::ResetCmd()
{
    qDebug() << "ResetCmd";
}

void ECMControl::ScoutCmd()
{
    qDebug() << "ScoutCmd";
}

void ECMControl::JammingCmd()
{
    qDebug() << "JammingCmd";
}

void ECMControl::_msgStandbyEncode()
{

}

void ECMControl::_msgResetEncode()
{

}

void ECMControl::_msgScoutCmd()
{

}

void ECMControl::_msgJammingCmd()
{

}
