#include "routeplanning.h"

RoutePlanning::RoutePlanning(QObject *parent)
    : QObject{parent}
{

}

void RoutePlanning::addSimpleItem(QGeoCoordinate coordinate)
{
    emit insertSimpleItem(coordinate);
}

void RoutePlanning::addTakeItem(QGeoCoordinate coordinate)
{
    emit insertTakeItem(coordinate);
}

void RoutePlanning::addLandItem(QGeoCoordinate coordinate)
{
    emit insertLandItem(coordinate);
}

void RoutePlanning::addCruiseItem(QGeoCoordinate coordinate)
{
    emit insertCruiseItem(coordinate);
}

void RoutePlanning::addLevelItem(QGeoCoordinate coordinate)
{
    emit insertLevelItem(coordinate);
}

void RoutePlanning::addHomewardItem(QGeoCoordinate coordinate)
{
    emit insertHomewardItem(coordinate);
}

void RoutePlanning::addEmergencyItem(QGeoCoordinate coordinate)
{
    emit insertEmergencyItem(coordinate);
}

void RoutePlanning::upload()
{
    emit uploadRoute();
}
