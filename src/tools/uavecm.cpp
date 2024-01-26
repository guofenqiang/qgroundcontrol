#include "uavecm.h"
#include "QGCApplication.h"

UAVEcm::UAVEcm(QObject *parent)
    : QObject{parent}
{
    initSignle();
}

void UAVEcm::initSignle()
{
    //由于有异步加载，所以获取根则必须使用objectCreated信号
    connect(qgcApp()->qmlAppEngine(), &QQmlApplicationEngine::objectCreated, this, &UAVEcm::slotGetRootEngine);
}

void UAVEcm::slotGetRootEngine()
{
    if(qgcApp()->qmlAppEngine()->rootObjects().isEmpty())
        return;
    QObject* rootWindowPointer = qgcApp()->qmlAppEngine()->rootObjects().first();
    ecm = rootWindowPointer->findChild<Ecm*>("ecm_obj");
    tcpServer = rootWindowPointer->findChild<TcpServer*>("tcp_server_obj");
    mavlinkPod = rootWindowPointer->findChild<MavlinkPod*>("mavlink_pod_obj");
    connect(ecm, &Ecm::sig_QLocalServer_to_mavlink, this, &UAVEcm::slotSendEcmToUav);
    connect(tcpServer, &TcpServer::sig_tcp_to_mavlink, this, &UAVEcm::slotSendEcmToUav);
    connect(mavlinkPod, &MavlinkPod::sig_mavlink_to_tcp, this, &UAVEcm::slotSendUavToEcm);
}

void UAVEcm::slotSendEcmToUav(QByteArray data)
{
    mavlinkPod->tcp_to_mavlink(data);
}

void UAVEcm::slotSendUavToEcm(QByteArray data)
{
    tcpServer->mavlink_to_tcp(data);
    ecm->mavlink_to_QLocalServer(data);
}
