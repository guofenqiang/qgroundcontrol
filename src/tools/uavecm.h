#ifndef UAVECM_H
#define UAVECM_H

#include "tools/ecm.h"
#include "tools/mavlinkpod.h"
#include "tools/tcpserver.h"
#include <QObject>

class UAVEcm : public QObject
{
    Q_OBJECT
public:
    explicit UAVEcm(QObject *parent = nullptr);

signals:

private slots:
    void slotGetRootEngine();
    void slotSendEcmToUav(QByteArray data);
    void slotSendUavToEcm(QByteArray data);

private:
    QObject* rootWindowPointer;
    Ecm *ecm = nullptr;
    TcpServer *tcpServer = nullptr;
    MavlinkPod *mavlinkPod = nullptr;

    void initSignle();
    void _handleTunnel(mavlink_message_t message);
};

#endif // UAVECM_H
