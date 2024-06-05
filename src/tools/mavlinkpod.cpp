#include "mavlinkpod.h"
#include "QGCApplication.h"

MavlinkPod::MavlinkPod(QObject *parent)
    : QObject{parent}
{
    _initSignle();
}

void MavlinkPod::_initSignle()
{
    connect(qgcApp()->toolbox()->mavlinkProtocol(), &MAVLinkProtocol::messageReceived, this, &MavlinkPod::_mavlinkMessageReceived);
}

void MavlinkPod::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
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

void MavlinkPod::_handleTunnel(mavlink_message_t message)
{
    mavlink_tunnel_t tunnel;

    mavlink_msg_tunnel_decode(&message, &tunnel);

    /* 0x0 代表雷达协议 */
    if (tunnel.payload_type != 0x0) {
        return;
    }

    QByteArray byte((char*)tunnel.payload, tunnel.payload_length);
    emit sig_mavlink_to_tcp(byte);
}

void MavlinkPod::tcp_to_mavlink(QByteArray data)
{
    mavlink_tunnel_t tunnel;
    Vehicle *_vehicle =  qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (_vehicle == NULL) {
        return;
    }
    WeakLinkInterfacePtr weakLink = _vehicle->vehicleLinkManager()->primaryLink();

    tunnel.target_system = static_cast<uint8_t>(_vehicle->id());
    tunnel.target_component = static_cast<uint8_t>(_vehicle->compId());
    tunnel.payload_type = 0;
    tunnel.payload_length = data.at(2);
    memcpy(tunnel.payload, (quint8*)data.data(), data.size());

    if (!weakLink.expired()) {
        SharedLinkInterfacePtr sharedLink = weakLink.lock();

        MAVLinkProtocol* mavlink = qgcApp()->toolbox()->mavlinkProtocol();
        mavlink_message_t msg;
        mavlink_msg_tunnel_encode_chan(
            static_cast<uint8_t>(mavlink->getSystemId()),
            static_cast<uint8_t>(mavlink->getComponentId()),
            sharedLink->mavlinkChannel(),
            &msg,
            &tunnel);
        _vehicle->sendMessageOnLinkThreadSafe(sharedLink.get(), msg);
    }
}
