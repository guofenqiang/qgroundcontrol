#include "ecmcontrol.h"
#include "QGCApplication.h"

ECMControl::ECMControl(QObject *parent)
    : QObject{parent}
    ,_scoutTargetInfo   (ScoutTargetInfo())
{
    _initSignle();
}

ECMControl::~ECMControl()
{
    if(pTimerJamming)
    {
        pTimerJamming->stop();
        delete pTimerJamming;
        pTimerJamming = nullptr;
    }
}

void ECMControl::_initSignle()
{
    curMode = STANDBY;
    connect(qgcApp()->toolbox()->mavlinkProtocol(), &MAVLinkProtocol::messageReceived, this, &ECMControl::_mavlinkMessageReceived);
    connect(this, &ECMControl::scoutResult, this, &ECMControl::_parseScoutResult);
    connect(this, &ECMControl::sigContinueScoutFlow, this, &ECMControl::slotContinueScout);
    //    timer.setInterval(1000);
//    connect(&timer, &QTimer::timeout, this, &ECMControl::_parseScoutResult);
//    timer.start();
}

int ECMControl::_generateRandom(int min, int max)
{
    return (rand() % (max - min + 1) + min); // 生成[min, max]范围内的随机整数
}

float ECMControl::_generateRandom(float min, float max)
{
    return (((float)rand() / RAND_MAX) * (max - min) + min);
}

void ECMControl::_sortQObjectsByNameLambda(QList<QObject*>& list) {
    std::sort(list.begin(), list.end(), [](QObject* obj1, QObject* obj2) {
        ScoutTargetInfo* scoutInfo1 = qobject_cast<ScoutTargetInfo*>(obj1);
        ScoutTargetInfo* scoutInfo2 = qobject_cast<ScoutTargetInfo*>(obj2);
        return scoutInfo1->timeScale() > scoutInfo2->timeScale();
    });
}

QList<QObject*> ECMControl::getList()
{
    int timeScale;
    float azimuth;
    float freq;
    float bandWidth;
    int signalPower;
    QList<QObject*> temp;

    for(int i = 0; i < _generateRandom(1, 5); i++)  //存放需要传送的链表，数目不定
    {
        timeScale = _generateRandom(1, 10000);
        azimuth = _generateRandom(0.0f, 359.0f);
        freq = _generateRandom(800.0f, 6000.0f);
        bandWidth = _generateRandom(1.0f, 300.0f);
        signalPower = _generateRandom(0, 3000);

        QObject* data = new ScoutTargetInfo(timeScale,
                                            round(azimuth),
                                            round(freq),
                                            round(bandWidth),
                                            signalPower);
        temp.append(data);
    }

    _sortQObjectsByNameLambda(temp);
    emit _scoutTargetInfo.signalList(temp); //将数据传入QML中，

    for (int i = temp.size() - 1; i >=0; --i) {
        dataHistList.push_front(temp[i]);
    }
    _sortQObjectsByNameLambda(dataHistList);

    return dataHistList;
}

void ECMControl::_parseScoutResult(scoutResult_t &scoutResult)
{
    int timeScale;
    float azimuth;
    float freq;
    float bandWidth;
    int signalPower;
    QList<QObject*> temp;

    for(int i = 0; i < scoutResult.scoutTargetNum; i++)  //存放需要传送的链表，数目不定
    {
        timeScale = scoutResult.targetInfo[i].timeScale;
        azimuth = scoutResult.targetInfo[i].azimuth;
        freq = scoutResult.targetInfo[i].freq;
        bandWidth = scoutResult.targetInfo[i].bandWidth;
        signalPower = scoutResult.targetInfo[i].signalPower;

        QObject* data = new ScoutTargetInfo(timeScale,
                                            round(azimuth),
                                            round(freq),
                                            round(bandWidth),
                                            signalPower);
        temp.append(data);
    }

    _sortQObjectsByNameLambda(temp);

    for (int i = temp.size() - 1; i >=0; --i) {
        dataHistList.push_front(temp[i]);
    }
    _sortQObjectsByNameLambda(dataHistList);
    emit _scoutTargetInfo.signalList(dataHistList); //将数据传入QML中，
}

ScoutTargetInfo* ECMControl::scoutTargetInfo()
{
    return &_scoutTargetInfo;
}

void ECMControl::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
{
    Vehicle *_vehicle = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle();
    if (_vehicle == NULL) {
        return;
    }

    Q_UNUSED(link);

    if (message.sysid != _vehicle->id()) {
        return;
    }

    switch (message.msgid) {
    case MAVLINK_MSG_ID_TUNNEL:
        _handleTunnel(message);
        break;
    }
}

void ECMControl::_handleTunnel(mavlink_message_t message)
{
    mavlink_tunnel_t tunnel;

    mavlink_msg_tunnel_decode(&message, &tunnel);

    QByteArray byte((char*)tunnel.payload, tunnel.payload_length);

    /* 0x0 代表雷达协议 */
    if (tunnel.payload_type == 0x0) {
        _message_decode(byte);
    }
}

void ECMControl::_message_decode(QByteArray data)
{
    ecm_message_t message = {};
    memcpy((uint8_t *)&message.stx, (uint8_t *)data.data(), data.size());

    switch (message.msgid) {
    case STANDBY_ACK:
    {
        _standbyCmdAck(message);
        break;
    }
    case RESET_ACK:
    {
        _resetCmdAck(message);
        break;
    }

    case SCOUT_ACK:
    {
        _scoutCmdAck(message);
        break;
    }
    case JAMMING_ACK:
    {
        _jammingCmdAck(message);
        break;
    }
    case SCOUT_RESULT:
    {
        _scoutResultAck(message);
        sigContinueScoutFlow();
        break;
    }
    default:
    {
        qDebug("invalid cmd: 0x%02x", message.msgid);
    }
    }
}

void ECMControl::_standbyCmdAck(ecm_message_t message)
{
    qDebug() << "_standbyCmdAck";
    setCurModeState("待机");
}
void ECMControl::_resetCmdAck(ecm_message_t message)
{
    qDebug() << "_resetCmdAck";
    setCurModeState("复位");
}
void ECMControl::_scoutCmdAck(ecm_message_t message)
{
    qDebug() << "_scoutCmdAck";
    setCurModeState("侦察");
}
void ECMControl::_jammingCmdAck(ecm_message_t message)
{
    qDebug() << "_jammingCmdAck";
    setCurModeState("干扰");
}

void ECMControl::_scoutResultAck(ecm_message_t message)
{
    scoutResult_t _scoutResult;
    memcpy((uint8_t *)&_scoutResult.scoutTargetNum, (uint8_t *)message.data, sizeof(_scoutResult));
    emit scoutResult(_scoutResult);
}

void ECMControl::standbyCmd()
{
    qDebug() << "StandbyCmd";
    ecm_message_t message = {};
    standby_message_t standby = {};

    curMode = STANDBY;

    _msgStandbyEncode(&message, &standby);
    _sendMessage(&message);
}

void ECMControl::resetCmd()
{
    qDebug() << "ResetCmd";

    ecm_message_t message = {};
    reset_message_t reset = {};

    curMode = RESET;

    _msgResetEncode(&message, &reset);
    _sendMessage(&message);
}

void ECMControl::scoutCmd()
{
    qDebug() << "ScoutCmd";
    ecm_message_t message = {};
    scout_message_t scout = {};

    curMode = SCOUT;
    if(pTimerJamming) {
        cleanJammingParams();
    }

    scout.scout_nums = 5;

    nFreqSegmentIdx = 0;
    scout.freq_c = 820 + (6000 - 800) / FREQ_SEGMENT_N * (nFreqSegmentIdx);
    _msgScoutCmd(&message, &scout);
    _sendMessage(&message);
    nFreqSegmentIdx++;
    nFreqSegmentIdx %= FREQ_SEGMENT_N;

//    scout.lon = _vehicle->longitude();
//    scout.lat = _vehicle->latitude();
//    scout.alt = _vehicle->altitudeRelative()->rawValue().toInt();
//    scout.eph = _scoutTargetInfo.azimuth();
}

void ECMControl::jammingCmd(quint16 lsb_freq, quint16 lsb_baud)
{
    qDebug() << "JammingCmd";
    ecm_message_t message = {};
    jamming_message_t jamming = {};

    jamming.lsb_freq = lsb_freq;
    jamming.lsb_baud = lsb_baud;

    curMode = JAMMING;
    _msgJammingCmd(&message, &jamming);
    _sendMessage(&message);

//    curJamming.append(jamming);

//    curMode = JAMMING;
//    jammingFreqIdx = 0;

//    jammingFlow();

//    if(pTimerJamming == nullptr) {
//        pTimerJamming = new QTimer();
//    }
//    else {
//        disconnect(pTimerJamming, &QTimer::timeout, this, &ECMControl::slotJammingTimerout);
//        pTimerJamming->stop();
//    }
//    connect(pTimerJamming, &QTimer::timeout, this, &ECMControl::slotJammingTimerout);
//    pTimerJamming->start(interval);
}

void ECMControl::_msgStandbyEncode(ecm_message_t *message, standby_message_t *standby)
{
    uint16_t length = sizeof(standby_message_t);
    memcpy(message->data, (uint8_t *)&standby->reserved[0], length);
    message->len = length + ECM_HEAD_LEN + sizeof(message->crc16);
    message->msgid = STANDBY;
    _message_encode(message);
}
void ECMControl::_msgResetEncode(ecm_message_t *message, reset_message_t *reset)
{
    uint16_t length = sizeof(reset_message_t);
    memcpy(message->data, (uint8_t *)&reset->reserved[0], length);
    message->len = length + ECM_HEAD_LEN + sizeof(message->crc16);
    message->msgid = RESET;
    _message_encode(message);
}

void ECMControl::_msgScoutCmd(ecm_message_t *message, scout_message_t *scout)
{
    uint16_t length = sizeof(scout_message_t);
    memcpy(message->data, (uint8_t *)&scout->scout_nums, length);
    message->len = length + ECM_HEAD_LEN + sizeof(message->crc16);
    message->msgid = SCOUT;
    _message_encode(message);
}

void ECMControl::_msgJammingCmd(ecm_message_t *message, jamming_message_t *jamming)
{
    uint16_t length = sizeof(jamming_message_t);
    memcpy(message->data, (uint8_t *)&jamming->lsb_freq, length);
    message->len = length + ECM_HEAD_LEN + sizeof(message->crc16);
    message->msgid = JAMMING;
    _message_encode(message);
}

void ECMControl::_message_encode(ecm_message_t *message)
{
    message->stx = 0x55AA;
    message->seq = 0;
    message->sysid = 0;
    message->crc16 = calculateChecksum_16((uint8_t*)&message->stx, message->len - sizeof(message->crc16));
}

uint16_t ECMControl::calculateChecksum_16(const uint8_t *data, const uint32_t size)
{
    const quint8* bytes = static_cast<const quint8*>(data);
    quint16 checksum = 0;
    for (quint32 i = 0; i < size; ++i) {
        checksum += bytes[i];
    }
    return checksum & 0xFFFF;
}

QByteArray ECMControl::structToByteArray(ecm_message_t *message)
{
    uint8_t len = message->len - sizeof(message->crc16);
    QByteArray dataBA(len, Qt::Uninitialized);
    memset(dataBA.data(), 0, len);
    memcpy(dataBA.data(), (char*)message, len);
    dataBA.append(message->crc16 & 0xFF);
    dataBA.append((message->crc16 >> 8) & 0xFF);
    return dataBA;
}

void ECMControl::_sendMessage(ecm_message_t *message)
{
    QByteArray buff;

    buff = structToByteArray(message);
    emit sig_control_to_mavlink(buff);
}

void ECMControl::slotContinueScout()
{
    if(curMode != SCOUT)
        return ;

    ecm_message_t message = {};
    scout_message_t scout = {};

    scout.scout_nums = 5;
    scout.freq_c = 820 + ((6000 - 800) / FREQ_SEGMENT_N) * (nFreqSegmentIdx);
    _msgScoutCmd(&message, &scout);
    _sendMessage(&message);

    nFreqSegmentIdx++;
    nFreqSegmentIdx %= FREQ_SEGMENT_N;

}

void ECMControl::jammingFlow()
{
    ecm_message_t message = {};
    jamming_message_t jamming = {};

    if (curJamming.size() == 0)
        qDebug() << "提示" <<  "请先选择干扰频点";
        return;

    jamming = curJamming.at(jammingFreqIdx);
    _msgJammingCmd(&message, &jamming);
    _sendMessage(&message);

    jammingFreqIdx++;
    jammingFreqIdx %= curJamming.size();
}

void ECMControl::slotJammingTimerout()
{
    if(curMode != JAMMING) {
        cleanJammingParams();
    }
    jammingFlow();
}

void ECMControl::cleanJammingParams()
{
    pTimerJamming->stop();
    pTimerJamming = nullptr;
    curJamming.clear();
}

QString ECMControl::getCurModeState() const
{
    return curModeState;
}

void ECMControl::setCurModeState(const QString &newCurModeState)
{
    if (curModeState == newCurModeState)
        return;
    curModeState = newCurModeState;
    emit curModeStateChanged();
}
