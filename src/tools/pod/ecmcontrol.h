#ifndef ECMCONTROL_H
#define ECMCONTROL_H

#include "tools/pod/scouttargetinfo.h"
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
} scout_message_t;

typedef struct _jamming_message_t {
    uint16_t lsb_freq;
    uint16_t lsb_baud;
    uint16_t reserved[4];
} jamming_message_t;

// 目标信息
typedef struct _scoutTargetInfo
{
    uint32_t    timeScale;        // 时标
    float       azimuth;           // 方位 rad		Yaw angle (-pi..+pi).
    float       freq;              // 频点  范围：0-6000MHz
    float       bandWidth;         // 带宽  范围：0-6000MHz
    uint32_t    signalPower;     // 信号强度
} scoutTargetInfo_t;


// 侦测结果
typedef struct _scoutResult
{
    uint8_t            scoutTargetNum;      // 侦查得到目标个数
    uint8_t            reserved;
    scoutTargetInfo_t  targetInfo[5];
} scoutResult_t;

#pragma pack(pop)

class ECMControl : public QObject
{
    Q_OBJECT
public:
    explicit ECMControl(QObject *parent = nullptr);
    ~ECMControl();

    Q_PROPERTY(ScoutTargetInfo* scoutTargetInfo READ scoutTargetInfo NOTIFY scoutTargetInfoChanged)
    Q_PROPERTY(QString curModeState READ getCurModeState WRITE setCurModeState NOTIFY curModeStateChanged)

    Q_INVOKABLE void standbyCmd();
    Q_INVOKABLE void resetCmd();
    Q_INVOKABLE void scoutCmd();
    Q_INVOKABLE void jammingCmd(quint16 lsb_freq, quint16 lsb_baud);

    ScoutTargetInfo* scoutTargetInfo();

    Q_INVOKABLE QList<QObject*> getList();

    QString getCurModeState() const;
    void setCurModeState(const QString &newCurModeState);

signals:
    void sig_control_to_mavlink(QByteArray data);

    void scoutTargetInfoChanged(ScoutTargetInfo &scoutTargetInfo);
    void scoutResult(scoutResult_t &scoutResult);
    void sigContinueScoutFlow();

    void curModeStateChanged();

private slots:
    void _mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message);
    void _parseScoutResult(scoutResult_t &scoutResult);
    void slotContinueScout();
    void slotJammingTimerout();

private:
#define ECM_HEAD_LEN    6
#define FREQ_SEGMENT_N 130
    enum ECM_CONTROL_CMD_ID{
        STANDBY = 0x0,
        RESET = 0x01,
        SCOUT = 0x02,
        JAMMING = 0x03,
        GPS_DATA = 0x04, //飞控->雷达信号处理板
    };

    enum ECM_CONTROL_CMD_ACK_ID {
        STANDBY_ACK = 0xa0,
        RESET_ACK = 0xa1,
        SCOUT_ACK = 0xa2,
        JAMMING_ACK = 0xa3,
        SCOUT_RESULT = 0xa4,
    };

    ScoutTargetInfo _scoutTargetInfo;
    QTimer timer;
    QList<QObject*> dataHistList;
    quint8  curMode;   // 记录当前模式
    quint8 nFreqSegmentIdx;
    quint8 jammingFreqIdx;
    QVector<jamming_message_t> curJamming;
    QTimer *pTimerJamming = nullptr;
    int interval = 100;
    QString curModeState;

    void _initSignle();
    void _handleTunnel(mavlink_message_t message);
    QByteArray structToByteArray(ecm_message_t *message);
    void _sendMessage(ecm_message_t *message);
    void _message_decode(QByteArray data);
    void _message_encode(ecm_message_t *message);
    uint16_t calculateChecksum_16(const uint8_t *data, const uint32_t size);
    void _msgStandbyEncode(ecm_message_t *message, standby_message_t *standby);
    void _msgResetEncode(ecm_message_t *message, reset_message_t *reset);
    void _msgScoutCmd(ecm_message_t *message, scout_message_t *scout);
    void _msgJammingCmd(ecm_message_t *message, jamming_message_t *jamming);

    void _standbyCmdAck(ecm_message_t message);
    void _resetCmdAck(ecm_message_t message);
    void _scoutCmdAck(ecm_message_t message);
    void _jammingCmdAck(ecm_message_t message);
    void _scoutResultAck(ecm_message_t message);

    QVariantList scoutTargetInfosToVariantList(const QList<scoutTargetInfo_t>& scoutTargetInfos);
    int _generateRandom(int min, int max);
    float _generateRandom(float min, float max);
    void _sortQObjectsByNameLambda(QList<QObject*>& list);

    void jammingFlow();
    void cleanJammingParams();
};

#endif // ECMCONTROL_H
