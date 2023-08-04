#ifndef PROTOCOLCONVERSION_H
#define PROTOCOLCONVERSION_H

#include <QObject>
#include "QByteArray"
#include "ProtocolType.h"
#include "mavlink.h"

class ProtocolConversion : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolConversion(QObject *parent = nullptr);

    void bz_protocol_conversion(QByteArray buff, mavlink_remote_cmd_t* remote_cmd);
    void CmdFeedback(mavlink_telemetry_cmd_t telemetry_cmd, QByteArray &buff);
    uint16_t crc16_xmodem(uint8_t *data, uint16_t length);
    void bz_encode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remote_cmd);
    void print_mavlink_message(bz_message_t *bz_message);
    void print_mavlink_message_ground_down(bz_message_ground_down_t *bz_message);
    void print_reciver(char *buff, uint8_t len);
    void print_send(QByteArray data);
signals:

};

#endif // PROTOCOLCONVERSION_H
