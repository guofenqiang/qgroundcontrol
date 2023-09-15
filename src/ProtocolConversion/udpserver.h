#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "qtimer.h"
#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>

class UDPServer : public QObject
{
    Q_OBJECT
public:
    explicit UDPServer(QObject *parent = nullptr);

    void InitSocket();//初始化套接字

    void InitTimer();
    void DeduplicationItem();

public slots:
    void ReadPendingDataframs();//读取消息

signals:

private:
    QUdpSocket *mUdpSocket;//UDP套接字
    QHostAddress mGroupAdress;//组播地址
    int mType; //记录UDP消息传送模式 0:单播 1:广播  2:组播（多播）
    QTimer _mTimer;
    QByteArray _pre_data;
    uint8_t _flag;
};

#endif // UDPSERVER_H
