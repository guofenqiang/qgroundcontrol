#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent)
    : QObject{parent}
{
    _initSignle();
}

void TcpServer::_initSignle()
{
    m_server = new QTcpServer(this);
    //由QTcplisten::listen 触发
    connect(m_server, &QTcpServer::newConnection, this, [=]() {
        m_tcp = m_server->nextPendingConnection();  //获得用于通信套接字
        qDebug() << "Successfully established a new connection with the client";
        qDebug() << m_tcp->peerAddress() << m_tcp->peerPort();

        //套接字 读任务 就绪
        connect(m_tcp, &QTcpSocket::readyRead, this, &TcpServer::_tcpMessageReceived);

        //套接字连接断开
        connect(
            m_tcp, &QTcpSocket::disconnected, this, [=]()
            {
                qDebug() << "The client has disconnected";
                m_tcp->deleteLater();
                /* QTcpSocket 是 QTcpServer的子对象，可以由QTcpServer自动析构(对象树) */
            });
    });

    m_server->listen(QHostAddress::Any, 9999);
}

void TcpServer::_tcpMessageReceived()
{
    QByteArray recvMsg = m_tcp->readAll();
    emit sig_tcp_to_mavlink(recvMsg);
}

void TcpServer::mavlink_to_tcp(QByteArray data)
{
    if(m_tcp == NULL) {
        return;
    }
    m_tcp->write(data);
}
