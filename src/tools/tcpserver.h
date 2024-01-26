#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);

    void mavlink_to_tcp(QByteArray data);

signals:
    void sig_tcp_to_mavlink(QByteArray data);

private slots:
    void _tcpMessageReceived();

private:
    QTcpServer* m_server;
    QTcpSocket* m_tcp = nullptr;

    void _initSignle();
};

#endif // TCPSERVER_H
