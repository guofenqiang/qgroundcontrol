#ifndef ROUTEPLANNING_H
#define ROUTEPLANNING_H

#include "qgeocoordinate.h"
#include <QObject>

class RoutePlanning : public QObject
{
    Q_OBJECT
public:
    explicit RoutePlanning(QObject *parent = nullptr);

    void addSimpleItem(QGeoCoordinate coordinate);
    void addTakeItem(QGeoCoordinate coordinate);
    void addLandItem(QGeoCoordinate coordinate);
    void addCruiseItem(QGeoCoordinate coordinate);
    void addLevelItem(QGeoCoordinate coordinate);
    void addHomewardItem(QGeoCoordinate coordinate);
    void addEmergencyItem(QGeoCoordinate coordinate);

    void upload();

signals:
    void insertSimpleItem(QGeoCoordinate coordinate);
    void insertTakeItem(QGeoCoordinate coordinate);
    void insertLandItem(QGeoCoordinate coordinate);
    void insertCruiseItem(QGeoCoordinate coordinate);
    void insertLevelItem(QGeoCoordinate coordinate);
    void insertHomewardItem(QGeoCoordinate coordinate);
    void insertEmergencyItem(QGeoCoordinate coordinate);

    void uploadRoute();
};

#endif // ROUTEPLANNING_H
