#ifndef MAVLINKPOD_H
#define MAVLINKPOD_H

#include "LinkInterface.h"
#include <QObject>

class MavlinkPod : public QObject
{
    Q_OBJECT
public:
    explicit MavlinkPod(QObject *parent = nullptr);

    void tcp_to_mavlink(QByteArray data);

signals:
    void sig_mavlink_to_tcp(QByteArray data);

private slots:
    void _mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message);

private:

    void _initSignle();
    void _handleTunnel(mavlink_message_t message);

};

#endif // MAVLINKPOD_H
