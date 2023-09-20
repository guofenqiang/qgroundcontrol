#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>
#include <QTimer>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = nullptr);
    virtual ~UDPClient();

    void InitSocket();//初始化UDP套接字

    void InitTimer();//初始化定时器

signals:

public slots:
    void SendData(QByteArray data);//发送数据

private:
    QUdpSocket *mUdpSocket;//UDP套接字对象
    QHostAddress mGroupAddress;//组播地址
    QTimer *mTimer;//定时器对象
    int mType;//记录UDP消息传送模式 0:单播 1:广播 2:组播（多播）

};

#endif // UDPCLIENT_H
