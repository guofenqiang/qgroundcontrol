#ifndef UDPSERVER_H
#define UDPSERVER_H

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

public slots:
    void ReadPendingDataframs();//读取消息

signals:

private:
    QUdpSocket *mUdpSocket;//UDP套接字
    QHostAddress mGroupAdress;//组播地址
    int mType; //记录UDP消息传送模式 0:单播 1:广播  2:组播（多播）
};

#endif // UDPSERVER_H
