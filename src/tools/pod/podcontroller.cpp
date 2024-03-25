#include "podcontroller.h"

PodController::PodController(QObject *parent)
    : QObject{parent}
{
    _podSiYiProtocol = new PodSiYiProtocol();
}

PodController::~PodController()
{

}

QByteArray PodController::hardware_id()
{
    _podSiYiProtocol->getHardwareId();
    return 0;
}

void PodController::mauanlFocus(int x, int y)
{
    qDebug() << "mauanlFocus" << x << " " << y;
    _podSiYiProtocol->zoom(x, y);
}
void PodController::sendRotateCommand(double yaw, double pitch)
{
    qDebug() << "sendRotateCommand" << yaw << " " << pitch;
    _podSiYiProtocol->rotate(yaw, pitch);
}
void PodController::oneClickReturnToCenter()
{
    qDebug() << "oneClickReturnToCenter";
    _podSiYiProtocol->oneClickReturnToCenter();
}
