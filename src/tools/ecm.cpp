#include "ecm.h"
#include <QProcess>
#include <QtConcurrent>
#include <QScreen>
#include "qlocalserver.h"

Ecm::Ecm(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_ANDROID
    initSubprocess();
#endif
//    initTimer();
}

Ecm::~Ecm()
{
    if(m_pServer != nullptr){
        delete m_pServer;
        m_pServer = nullptr;
    }
    if (videoProcess->state() != QProcess::NotRunning) {
        videoProcess->terminate(); // 尝试终止进程
        videoProcess->waitForFinished(3000); // 等待最多3秒
        if (videoProcess->state() != QProcess::NotRunning) {
            videoProcess->kill(); // 如果进程仍然没有终止，强制结束它
        }
    }
}

void Ecm::initSubprocess()
{
    //初始化子进程
    videoProcess = new QProcess(this);
    connect(videoProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Ecm::slotClientFinished);
    //初始化主进程服务端
    m_pServer = new QLocalServer(this);
    //监听客户端
    m_pServer->removeServer("UAV_ECM_Pod");
    if (!m_pServer->listen("UAV_ECM_Pod")) {
        qCritical() << QString("Local Fortune Server: Unable to start the server: %1.").arg(m_pServer->errorString());
        //        close();
        return;
    }
    connect(m_pServer, &QLocalServer::newConnection,
            this, &Ecm::slotNewConnected);
#ifdef Q_OS_WIN
    QString program = "D:/code/other_code/ECM/UAV_ECM_Pod_20240103/build-UAV_ECM_Pod-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/bin/UAV_ECM_Pod.exe";
    QStringList argument;
    videoProcess->start(program, argument, QProcess::ReadWrite);
    videoProcess->waitForStarted();
#endif

#ifdef Q_OS_ANDROID
    //    QString program = "/bin/bash";
    //    QStringList argument("am start -n org.qtproject.example.UAV_ECM_Pod/org.qtproject.qt5.android.bindings.QtActivity");
    //    videoProcess->start("am", QStringList() << "start" << "-n" << "org.qtproject.example.UAV_ECM_Pod/org.qtproject.qt5.android.bindings.QtActivity");
    //    videoProcess->start("am start -n org.qtproject.example.UAV_ECM_Pod/org.qtproject.qt5.android.bindings.QtActivity");

    videoProcess->setProgram("shell");
    videoProcess->setArguments({"-c", "am start -n org.qtproject.example.UAV_ECM_Pod/org.qtproject.qt5.android.bindings.QtActivity"});
    videoProcess->start();
    qDebug() << videoProcess->waitForStarted();
#endif

    //    videoProcess = new QProcess(this);
    //    connect(videoProcess,&QProcess::readyReadStandardOutput,this,&Ecm::slot_readyStandandOutput);
    //    videoProcess->start(s);

}

void Ecm::slotClientFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)

    m_bOpenClient = false;
}

void Ecm::slotNewConnected()
{
    m_pClient = m_pServer->nextPendingConnection();
    if(m_pClient)
    {
        m_bOpenClient = true;
        connect(m_pClient, &QLocalSocket::disconnected,
                m_pClient, &QLocalSocket::deleteLater);

        m_uavDataOut.setDevice(m_pClient);
        m_uavDataOut.setVersion(QDataStream::Qt_5_15);
        connect(m_pClient, &QLocalSocket::readyRead,
                this, &Ecm::slotReadLocalSocket);

    }
}

void Ecm::slotReadLocalSocket()
{
    QByteArray ang;
    m_uavDataOut >> ang;
    emit sig_QLocalServer_to_mavlink(ang);
}


void Ecm::slotSendToClient()
{
    if(!m_bOpenClient)
        return;

    if(m_pClient)
    {
        QByteArray byteIn;
        byteIn.resize(rand() % 10);
        for(int i = 0; i < byteIn.size(); ++i) {
            byteIn[i] = rand() % 0xFF;
        }

        QByteArray block;
        QDataStream serverDataOut(&block, QIODevice::WriteOnly);
        serverDataOut.setVersion(QDataStream::Qt_5_10);
        serverDataOut << byteIn;

        if(m_pClient->state() == QLocalSocket::ConnectedState) {
            m_pClient->write(block);
            m_pClient->flush();
        }
    }
}

void Ecm::initTimer()
{
    _JoystickTimeUpdater.setInterval(1000);
//    connect(&_JoystickTimeUpdater, &QTimer::timeout, this, &Ecm::slot_pushButton_clicked);
    connect(&_JoystickTimeUpdater, &QTimer::timeout, this, &Ecm::slotSendToClient);
    _JoystickTimeUpdater.start();
}

void Ecm::add()
{
    qDebug() << "add";
}

void Ecm::reduce()
{
    qDebug() << "reduce";
}

void Ecm::slot_readyStandandOutput()
{
    QByteArray log = videoProcess->readAllStandardOutput();
    log = "[main_process reciver]:" + log;
    qDebug() << log;
}

void Ecm::slot_pushButton_clicked()
{
    static quint8 count = 0;
    count++;

    QByteArray byteArray(reinterpret_cast<const char*>(&count), sizeof(count));
    if(videoProcess->Running){
        videoProcess->write(byteArray);
    }
}

void Ecm::mavlink_to_QLocalServer(QByteArray data)
{
    if(!m_bOpenClient)
        return;

    if(m_pClient)
    {
        QByteArray block;
        QDataStream serverDataOut(&block, QIODevice::WriteOnly);
        serverDataOut.setVersion(QDataStream::Qt_5_10);
        serverDataOut << data;

        if(m_pClient->state() == QLocalSocket::ConnectedState) {
            m_pClient->write(block);
            m_pClient->flush();
        }
    }
}
