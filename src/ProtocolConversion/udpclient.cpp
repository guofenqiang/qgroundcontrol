#include "udpclient.h"
#include "protocolconversion.h"

UDPClient::UDPClient(QObject *parent)
    : QObject{parent}
{
    mType = 2;//Multicast
    InitSocket();
//    InitTimer();
}

void UDPClient::InitSocket()
{
    mUdpSocket = new QUdpSocket;//初始化socket
    mGroupAddress.setAddress("224.0.0.02");//设置组播地址
    mUdpSocket->bind(1520);//绑定本地端口号
    if(mType == 2)
    {
        //组播的数据的生存期，数据报没跨1个路由就会减1.表示多播数据报只能在同一路由下的局域网内传播
        mUdpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);
    }
}

void UDPClient::InitTimer()
{
    mTimer = new QTimer;//初始化定时器
    connect(mTimer,&QTimer::timeout,this,[=]
            {
                SendData("hello world");
            });
    mTimer->start(1000);//每隔一秒发送一次数据
}

void UDPClient::SendData(QByteArray data)
{
    if(mType == 0)//单播
    {
        QHostAddress _peerHostAddress = QHostAddress("10.0.0.177");//对位服务器IP
        quint16 _port = 6666;//对位服务器端口
        if(-1 !=mUdpSocket->writeDatagram(data.data(),data.size(),_peerHostAddress,_port))
        {
            qDebug()<< "Unicast ==> Send data : "<< data;
        }
        mUdpSocket->flush();
    }
    else if(mType == 1)//广播
    {
        quint16 _port = 6666;//广播端口
        if(-1 !=mUdpSocket->writeDatagram(data.data(),QHostAddress::Broadcast,_port))
        {
            qDebug()<< "Broadcast ==> Send data : "<< data;
        }
        mUdpSocket->flush();
    }
    else if(mType == 2)//组播
    {
        quint16 _port = 7043;//组播端口
        ProtocolConversion _ptconv;

        uint8_t *buff = (uint8_t *)malloc(data.size() * sizeof(uint8_t));
        int len = 115;

//        _ptconv.print_send(data);
        _ptconv.qbyte_array_to_char(buff, &len, data);

        if(-1 != mUdpSocket->writeDatagram((char*)buff, len, mGroupAddress, _port))
        {
            qDebug()<< "Multicast ==> Send data : "<< data.toHex();
        }
        mUdpSocket->flush();
    }
    else
    {
        qDebug()<< "mType is error! ";
        return;
    }


}

