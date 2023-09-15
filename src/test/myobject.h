#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
//#include <Qtqml>
#include "mavlink_types.h"
#include "qqml.h"

class MyObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit MyObject(QObject *parent = nullptr);

    static MyObject * getInstance();

    int iValue() const;
    void setIValue(int newIValue);

    QString sString() const;
    void setSString(const QString &newSString);

    Q_INVOKABLE void func();
    Q_INVOKABLE void _radarCmd(int cmd);
    void _handleRadarCmdAck(mavlink_message_t& message);
    void _handleRadarMeasure(mavlink_message_t& message);

public slots:
    void cppSlot(int i, QString s);

signals:

    void iValueChanged();

    void sStringChanged();

    void cppSig(QVariant i, QVariant s);

private:
    int m_iValue;
    QString m_sString;

    Q_PROPERTY(int iValue READ iValue WRITE setIValue NOTIFY iValueChanged)
    Q_PROPERTY(QString sString READ sString WRITE setSString NOTIFY sStringChanged)

};

#endif // MYOBJECT_H
