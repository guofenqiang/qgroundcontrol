#ifndef ECMCONTROL_H
#define ECMCONTROL_H

#include "tools/mavlinkpod.h"
#include <QObject>

;
#pragma pack(push)
#pragma pack(1)
typedef struct _ecm_message_t {
    uint16_t stx;
    uint8_t len;
    uint8_t seq;
    uint8_t sysid;
    uint8_t msgid;
    uint8_t data[119];
    uint16_t crc16;
} ecm_message_t;

typedef struct _standby_message_t {
    uint8_t reserved[4];
} standby_message_t;

typedef struct _reset_message_t {
    uint8_t reserved[4];
} reset_message_t;

typedef struct _scout_message_t {
    uint8_t scout_nums;
    uint8_t reserved0;
    uint16_t freq_c;
    uint16_t reserved1;
    int32_t lon;
    int32_t lat;
    int32_t alt;
    uint16_t eph;
    uint8_t reserved2[2];
} scoutg_message_t;

typedef struct _jamming_message_t {
    uint16_t lsb_freq;
    uint16_t lsb_baud;
    uint16_t reserved[4];
} jamming_message_t;

#pragma pack(pop)

class ECMControl : public QObject
{
    Q_OBJECT
public:
    explicit ECMControl(QObject *parent = nullptr);

    Q_INVOKABLE void StandbyCmd();
    Q_INVOKABLE void ResetCmd();
    Q_INVOKABLE void ScoutCmd();
    Q_INVOKABLE void JammingCmd();

signals:

private slots:
    void _mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message);

private:
    enum ECM_CONTROL_CMD_ID{
        STANDBY = 0x0,
        RESET = 0x01,
        SCOUT = 0x02,
        JAMMING = 0x03,
    };
    MavlinkPod *mavlinkPod = nullptr;

    void _initSignle();
    void _handleTunnel(mavlink_message_t message);
    void _msgStandbyEncode();
    void _msgResetEncode();
    void _msgScoutCmd();
    void _msgJammingCmd();
};

#endif // ECMCONTROL_H
