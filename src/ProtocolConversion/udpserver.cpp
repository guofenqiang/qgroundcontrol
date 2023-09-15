#include "udpserver.h"
//#include "BZ_Convertion/BZConv.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "QGCApplication.h"
#include "protocolconversion.h"
#include <QTimer>

UDPServer::UDPServer(QObject *parent)
    : QObject{parent}
{
    mType = 2;//Multicast
    InitSocket();
    InitTimer();
}

void UDPServer::InitSocket()
{
    //初始化socket，设置组播地址
    mUdpSocket = new QUdpSocket;
    mGroupAdress.setAddress("224.0.0.03");
    quint16 _local_udp_port = 7044;

    if(mType == 0 || mType == 1)
    {
        //绑定本地IP和端口号
        mUdpSocket->bind(6666);
    }
    else if(mType == 2)
    {
        if(mUdpSocket->bind(QHostAddress::AnyIPv4, _local_udp_port, QUdpSocket::ShareAddress))
        {
            //加入组播地址
            mUdpSocket->joinMulticastGroup(mGroupAdress);
            qDebug()<<("Join Multicast Adrress [")<<mGroupAdress.toString()
                     <<("] Successful!");
        }
    }
    else
    {
        qDebug()<< "mType is error! ";
        return;
    }

    connect(mUdpSocket,&QUdpSocket::readyRead,this,[=]{
        ReadPendingDataframs();
    });
}



void UDPServer::ReadPendingDataframs()
{
    auto *vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    QByteArray _data;
    ProtocolConversion _ptconv;
    mavlink_remote_cmd_t remote_cmd = {0};

    _data.resize(mUdpSocket->pendingDatagramSize());
    if(mType == 0)//Unicast
    {
        QHostAddress *_peerHostAddress = new QHostAddress("10.0.0.32");
        quint16 _port = 6666;
        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),_peerHostAddress,&_port);//接收指定IP和端口的udp报文
            qDebug()<<"Unicast ==> Receive data : "<<QString::fromLatin1(_data);
        }
    }
    else if(mType == 1)//Broadcast
    {
        QHostAddress _peerHostAddress;
        quint16 _port;
        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),&_peerHostAddress,&_port);//接收同一子网的udp报文
            qDebug()<<"Broadcast ==> Receive data : "<<QString::fromLatin1(_data);
        }
    }
    else if(mType == 2)//Multicast
    {
        QHostAddress _peerHostAddress;
        quint16 _port;

        while(mUdpSocket->hasPendingDatagrams())
        {
            mUdpSocket->readDatagram(_data.data(),_data.size(),&_peerHostAddress,&_port);//接收同组的udp报文
//            qDebug()<<"Multicast ==> Receive data : "<<QString::fromLatin1(_data);
            if (_flag == 0) {
                _mTimer.start(10);
                _flag = 1;
            }
            if (_data.toHex() == _pre_data.toHex()) {
                continue;
            }

            qDebug()<<"Multicast ==> Receive data : "<<_data.toHex();
            _ptconv.bz_protocol_conversion(_data, &remote_cmd);
            _ptconv.bz_telecontrol_decode(remote_cmd);
            vehicle->remoteCmd(remote_cmd);
            _pre_data = _data;
        }
    }
    else
    {
        qDebug()<< "mType is error! ";
        return;
    }
}

void UDPServer::InitTimer()
{
    connect(&_mTimer, &QTimer::timeout, this, &UDPServer::DeduplicationItem);
    _flag = 0;
}

void UDPServer::DeduplicationItem()
{
    _pre_data.clear();
    _flag = 0;
}

