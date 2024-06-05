#include "podsiyiprotocol.h"
#include <QHostAddress>

PodSiYiProtocol::PodSiYiProtocol(QObject *parent)
    : QObject{parent}
{
    _udpSocket = new QUdpSocket(this);

    connect(_udpSocket, &QUdpSocket::connected, this, &PodSiYiProtocol::connected);
    connect(_udpSocket, &QUdpSocket::disconnected, this, &PodSiYiProtocol::disconnected);
//    connect(_udpSocket, &QUdpSocket::readyRead, this, &PodSiYiProtocol::readyRead);

    _connectToPod(POD_SIYI_A8_MINI_IP_ADDR, POD_SIYI_A8_MINI_IP_PORT);
}

PodSiYiProtocol::~PodSiYiProtocol()
{

}

void PodSiYiProtocol::connected()
{
    qDebug() << "Connected to server";
    getHardwareId();
}

void PodSiYiProtocol::disconnected()
{
    qDebug() << "Disconnected from server";
}

void PodSiYiProtocol::readyRead()
{
    QByteArray data = _udpSocket->readAll();
    _siyi_message_decode(data);
}

bool PodSiYiProtocol::_connectToPod(const QString &ipAddress, quint16 port)
{
    _udpSocket->bind();
    return true;
}

void PodSiYiProtocol::_siyi_message_decode(QByteArray data)
{
    siyi_message_t message;
    memcpy((uint8_t *)&message.stx, (uint8_t *)data.data(), data.size());

    switch (message.cmd_id) {
    case HARDWARE_ID:
    {
        _getHardwareIdAck(message);
        break;
    }
    case ZOOM_FOUCS:
    {
        _zoomAck(message);
        break;
    }

    case PAN_TILT_ROTATION:
    {
        _rotateAck(message);
        break;
    }
    case ONE_RETURN_CENTER:
    {
        _oneClickReturnToCenterAck(message);
        break;
    }
    }
}

void PodSiYiProtocol::_siyi_message_encode(siyi_message_t *message)
{
    message->stx = 0x6655;
    message->ctrl = 0x01;
    message->seq = 0;
    message->crc16 = _CRC16_cal((uint8_t*)&message->stx, message->len + HEAD_LEN, 0);
}

/***********************************************************
CRC16 Coding & Decoding G(X) = X^16+X^12+X^5+1
***********************************************************/
uint16_t PodSiYiProtocol::_CRC16_cal(uint8_t *ptr, uint32_t len, uint16_t crc_init)
{
    uint16_t crc, oldcrc16;
    uint8_t temp;
    crc = crc_init;
    while (len--!=0)
    {
        temp=(crc>>8)&0xff;
        oldcrc16 = crc16_tab[*ptr^temp];
        crc=(crc<<8)^oldcrc16;
        ptr++;
    }
    //crc=~crc; //??
    return(crc);
}

uint8_t PodSiYiProtocol::_crc_check_16bites(uint8_t *pbuf, uint32_t len, uint32_t *p_result)
{
    uint16_t crc_result = 0;
    crc_result= _CRC16_cal(pbuf,len, 0);
    *p_result = crc_result;
    return 2;
}

void PodSiYiProtocol::_message_to_qbyte(char *buff, int64_t *len, siyi_message_t *msg)
{
    memcpy(buff, (char*)&msg->stx, msg->len + HEAD_LEN);
    /* 在buff后面添加crc校验 */
    memcpy((uint8_t *)&buff[msg->len + HEAD_LEN], (uint8_t *)&msg->crc16, sizeof(msg->crc16));
    *len = msg->len + HEAD_LEN + sizeof(msg->crc16);
}

void PodSiYiProtocol::_sendMessage(siyi_message_t *message)
{
    char buff[300];
    int64_t len;

//    qDebug("0x%04x", message->stx);
//    qDebug("0x%02x", message->ctrl);
//    qDebug("0x%04x", message->len);
//    qDebug("0x%04x", message->seq);
//    qDebug("0x%02x", message->cmd_id);
//    for(int i = 0; i < message->len; i++)
//    {
//        qDebug("0x%02x", message->data[i]);
//    }
//    qDebug("0x%04x", message->crc16);
    _message_to_qbyte(buff, &len, message);
    _udpSocket->writeDatagram(buff, len, QHostAddress(POD_SIYI_A8_MINI_IP_ADDR), POD_SIYI_A8_MINI_IP_PORT);
}

void PodSiYiProtocol::_msgGetHardwareIdEncode(siyi_message_t *message)
{
    message->len = 0;
    message->cmd_id = HARDWARE_ID;

    _siyi_message_encode(message);
}

void PodSiYiProtocol::_msgZoomEncode(siyi_message_t *message, zoom_message_t *zoom)
{
    uint16_t length = sizeof(zoom_message_t);

    memcpy(message->data, (uint8_t *)&zoom->zoom, length);
    message->len = length;
    message->cmd_id = ZOOM_FOUCS;
    _siyi_message_encode(message);
}

void PodSiYiProtocol::_msgRotateEncode(siyi_message_t *message, rotate_message_t *rotate)
{
    uint16_t length = sizeof(rotate_message_t);

    memcpy(message->data, (uint8_t *)&rotate->turn_yaw, length);
    message->len = length;
    message->cmd_id = PAN_TILT_ROTATION;
    _siyi_message_encode(message);
}

void PodSiYiProtocol::_msgCenterEncode(siyi_message_t *message, oneClickReturnToCenter_message_t *center)
{
    uint16_t length = sizeof(oneClickReturnToCenter_message_t);

    memcpy(message->data, (uint8_t *)&center->center_pos, length);
    message->len = length;
    message->cmd_id = ONE_RETURN_CENTER;
    _siyi_message_encode(message);
}

void PodSiYiProtocol::getHardwareId()
{
    siyi_message_t message;

    _msgGetHardwareIdEncode(&message);
    _sendMessage(&message);
}

void PodSiYiProtocol::zoom(int32_t x, int32_t y)
{
    siyi_message_t message;
    zoom_message_t zoom;

    if (x > 0 || y > 0) {
        zoom.zoom = 1;
    } else if (x < 0 || y < 0) {
        zoom.zoom = -1;
    }

    _msgZoomEncode(&message, &zoom);
    _sendMessage(&message);
    /* 取消滚轮 */
    zoom.zoom = 0;
    _msgZoomEncode(&message, &zoom);
    _sendMessage(&message);
}

void PodSiYiProtocol::rotate(double yaw, double pitch)
{
    siyi_message_t message;
    rotate_message_t rotate;

    if (yaw > 100) {
        yaw = 100;
    }

    if (yaw < -100) {
        yaw = -100;
    }

    if (pitch > 100) {
        pitch = 100;
    }

    if (pitch < -100) {
        pitch = -100;
    }

    if (yaw == 0 && pitch == 0) {
        yaw = 0;
        pitch = 0;
    }

    rotate.turn_yaw = (int)yaw;
    rotate.turn_pitch = -(int)pitch;

    _msgRotateEncode(&message, &rotate);
    _sendMessage(&message);
}

void PodSiYiProtocol::oneClickReturnToCenter()
{
    siyi_message_t message;
    oneClickReturnToCenter_message_t center;

    center.center_pos = 0x1;

    _msgCenterEncode(&message, &center);
    _sendMessage(&message);
}

void PodSiYiProtocol::_getHardwareIdAck(siyi_message_t message)
{
    qDebug() << "_getHardwareIdAck";
    memcpy((uint8_t *)&_hardware.hardware_id[0], (uint8_t *)&message.data[0], message.len);
    qDebug() << _hardware.hardware_id[0];

    switch (_hardware.hardware_id[0]) {
    case 0x6B:
    {
        qDebug() << "ZR10";
    }
    case 0x73:
    {
        qDebug() << "A8 mini";
    }
    case 0x75:
    {
        qDebug() << "A2 mini";
    }
    case 0x78:
    {
        qDebug() << "ZR30";
    }
    case 0x7A:
    {
        qDebug() << "ZT30";
    }
    }
}

void PodSiYiProtocol::_zoomAck(siyi_message_t message)
{
    qDebug() << "_zoomAck";
    zoom_message_ack_t ack;

    memcpy((uint8_t *)&ack.zoom_multiple, (uint8_t *)&message.data[0], message.len);
    qDebug() << ack.zoom_multiple;
}

void PodSiYiProtocol::_rotateAck(siyi_message_t message)
{
    qDebug() << "_rotateAck";

    rotate_message_ack_t ack;
    memcpy((uint8_t *)&ack.sta, (uint8_t *)&message.data[0], message.len);
    qDebug() << ack.sta;
}

void PodSiYiProtocol::_oneClickReturnToCenterAck(siyi_message_t message)
{
    qDebug() << "_oneClickReturnToCenterAck";

    oneClickReturnToCenter_message_ack_t ack;
    memcpy((uint8_t *)&ack.sta, (uint8_t *)&message.data[0], message.len);
    qDebug() << ack.sta;
}
