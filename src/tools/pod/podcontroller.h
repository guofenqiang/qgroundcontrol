#ifndef PODCONTROLLER_H
#define PODCONTROLLER_H

#include <QObject>
#include "tools/pod/siyi/podsiyiprotocol.h"

class PodController : public QObject
{
    Q_OBJECT
public:
    explicit PodController(QObject *parent = nullptr);   
    ~PodController();

    QByteArray hardware_id();
    Q_INVOKABLE void mauanlFocus(int x, int y);
    Q_INVOKABLE void sendRotateCommand(double x, double y);
    Q_INVOKABLE void oneClickReturnToCenter();

signals:

private slots:

private:
    PodSiYiProtocol *_podSiYiProtocol;

};

#endif // PODCONTROLLER_H
