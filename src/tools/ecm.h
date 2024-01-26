#ifndef ECM_H
#define ECM_H

#include <QDataStream>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QProcess>
#include <QTimer>

class Ecm : public QObject
{
    Q_OBJECT
public:
    explicit Ecm(QObject *parent = nullptr);
    ~Ecm();

    Q_INVOKABLE void add();
    Q_INVOKABLE void reduce();

    void mavlink_to_QLocalServer(QByteArray data);

public slots:
    void slotNewConnected();
    void slotReadLocalSocket();
signals:
    void sig_QLocalServer_to_mavlink(QByteArray data);

private slots:
    void slot_readyStandandOutput();
    void slot_pushButton_clicked();

    void slotClientFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotSendToClient();
private:
    QProcess *videoProcess;
    QTimer      _JoystickTimeUpdater;

    void print_log(QString &log);
    void initSubprocess();
    void initTimer();

    bool m_bOpenClient = false;        //子进程打开标志
    QLocalServer    *m_pServer;     //主进程服务器
    QLocalSocket    *m_pClient = nullptr;  //子进程客户端

    QDataStream         m_uavDataOut;   // 信处板发送的数据流

};

#endif // ECM_H
