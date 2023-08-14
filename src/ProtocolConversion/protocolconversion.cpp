#include "protocolconversion.h"
#include "QGCApplication.h"
#include "BZ_Convertion/BZConv.h"
#include "BZ_Convertion/CRCCheck.h"

ProtocolConversion::ProtocolConversion(QObject *parent)
    : QObject{parent}
{

}


void ProtocolConversion::bz_protocol_conversion(QByteArray buff, mavlink_remote_cmd_t* remote_cmd)
{
    uint8_t UAVBuffer[MAVLINK_MSG_ID_REMOTE_CMD_LEN];
    uint8_t len = buff.size();
    uint8_t *array = (uint8_t *)malloc(len * sizeof(uint8_t));

    memcpy(array, buff.data(), len);
    BZ_Ground2UAV(UAVBuffer, array);
    memcpy((uint8_t*)&remote_cmd->telecontrol[0], UAVBuffer, MAVLINK_MSG_ID_REMOTE_CMD_LEN);

    free(array);
    return;
}

void ProtocolConversion::CmdFeedback(mavlink_telemetry_cmd_t telemetry_cmd, QByteArray& buff)
{
    uint8_t grdBuffer[115];
    uint8_t UAVBuffer[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN];
    static drone_platform_status_feedback_data_t feedback_data = {0};
    uint16_t crc;

    feedback_data.flight_control_version++;
    feedback_data.hw_version++;
    feedback_data.longitude++;
    feedback_data.latitude++;
    feedback_data.relative_height++;
    feedback_data.yaw_angle++;
    feedback_data.pitch_angle++; //偏航角
    feedback_data.roll_angle++; //俯仰角
    feedback_data.northbound_velocity++; //横滚角
    feedback_data.westbound_velocity++;
    feedback_data.vertical_velocity++;
    feedback_data.ground_speed++;
    feedback_data.battery_voltage++;
    feedback_data.percentage_of_remaining_battery_power++;
    feedback_data.flight_status++;
    feedback_data.satellite_positioning_mode++;
    feedback_data.loss_satellite_time++;
    feedback_data.airline_bus++;
    feedback_data.total_number_of_routes++;
    feedback_data.current_route_number++;
    feedback_data.total_number_of_waypoints++;
    feedback_data.current_waypoint_number++;
    feedback_data.longitude_of_the_next_waypoint++;
    feedback_data.latitude_of_the_next_waypoint++;
    feedback_data.relative_altitude_of_the_next_waypoint++;
    feedback_data.autonomous_obstacle_avoidance_state++;
    feedback_data.platform_status++;
    feedback_data.flight_time++;
    feedback_data.startup_time++;
    feedback_data.flight_confirmation++;

//    bz_message = (bz_message_t *)data;

//    bz_message->frame_header1 = 0xA5;
//    bz_message->frame_header2 = 0x5A;
//    bz_message->frame_length = MAVLINK_MSG_ID_TELEMETRY_CMD_LEN;

//    bz_message->seq = 0x33;
//    bz_message->sender_sysid = 0xff;
//    bz_message->receiver_sysid = 0x01;

//    bz_message->cmd0 = telemetry_cmd.telemetry[7];
//    bz_message->cmd1 = telemetry_cmd.telemetry[8];

//    memcpy(&bz_message->pyload[0], (uint8_t*)&telemetry_cmd.telemetry[9], MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 12);
//    bz_message->frame_tail = 0xAA;

    memcpy(UAVBuffer, (uint8_t*)&(telemetry_cmd.telemetry[0]), MAVLINK_MSG_ID_TELEMETRY_CMD_LEN);
    UAVBuffer[7] = 0x10;
    UAVBuffer[8] = 0x31;
    memcpy((uint8_t*)&UAVBuffer[9], (uint8_t*)&feedback_data.flight_control_version, 63-9+1);

    crc = GetCRC16((uint8_t*)&UAVBuffer[0], MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 3);
    UAVBuffer[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 3] = crc & 0x00ff;
    UAVBuffer[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 2] = crc >> 0x8;

    if(int ret = BZ_UAV2Ground(grdBuffer, UAVBuffer)) {
        qDebug() << "BZ_UAV2Ground filed" << ret;
    }

    for(int i = 0; i < 115; i++) {
        buff[i] = grdBuffer[i];
    }

    return;
}

void ProtocolConversion::bz_encode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
//    typedef struct __command_feedback_response{
//        uint8_t reciver_status;
//        uint8_t cmd0;
//        uint8_t cmd1;
//        uint8_t sender_id;
//        uint8_t reciver_id;
//        uint16_t reciver_time;
//        uint8_t exe_status;
//    } command_feedback_response_t;
//    command_feedback_response_t cmd_feedback;

    switch ((remoteCmd.telecontrol[7] << 8) | remoteCmd.telecontrol[8]) {
        /* 遥控命令子定义*/
        case WORK_MODE:
            work_mode(telemetryCmd, remoteCmd);
            break;
        case LIGHT_SWITCHING:
            light_switching(telemetryCmd, remoteCmd);
            break;
        case IMAGE_STABILIZATION_INSTRUCTION:
            image_stabilization_instruction(telemetryCmd, remoteCmd);
            break;
        case ZOOM:
            zoom(telemetryCmd, remoteCmd);
            break;
        case LASER_IRRADIATION:
            laser_irradiattion(telemetryCmd, remoteCmd);
            break;
        case PHOTOGRAPHY:
            photography(telemetryCmd, remoteCmd);
            break;
        case CENTERING:
            centering(telemetryCmd, remoteCmd);
            break;
        case VIDEO:
            video(telemetryCmd, remoteCmd);
            break;
        case TARGET_DETECTION_AND_RECOGNITION:
            target_detection_and_recognition(telemetryCmd, remoteCmd);
            break;
        case AUTOMATIC_TARGET_TRACKING:
            automatic_target_tracking(telemetryCmd, remoteCmd);
            break;
        case PAN_TILT_CONTROL_MODE:
            pan_tilt_control_mode(telemetryCmd, remoteCmd);
            break;
        case OPTOELECTRONIC_LOAD_STATUS_QUERY:
            optoelectronic_load_status_query(telemetryCmd, remoteCmd);
            break;

        case GJ_WORK_MODE:
            GJ_work_mode(telemetryCmd, remoteCmd);
            break;
        case ATTACK_MODE_SWITCHING:
            attack_mode_switching(telemetryCmd, remoteCmd);
            break;
        case INSURANCE_MODE:
            insurance_mode(telemetryCmd, remoteCmd);
            break;
        case ATTACK_STATUS:
            attack_status(telemetryCmd, remoteCmd);
            break;
        case GJ_AUTOMATIC_TARGET_TRACKING:
            GJ_automatic_target_tracking(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_STRIKE:
            autonomous_strike(telemetryCmd, remoteCmd);
            break;
        case GJ_LOAD_STATUS_QUERY:
            GJ_load_status_query(telemetryCmd, remoteCmd);
            break;
        case ZDB_CONTROL:
            ZDB_control(telemetryCmd, remoteCmd);
            break;

        case VIRTUAL_ROCKER_MODE:
            virtual_rocker_mode(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_TAKEOFF:
            autonomous_takeoff(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_RETURN:
            autonomous_return(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_CRUISE:
            autonomous_cruise(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_FLIGHT_AND_STEERING:
            autonomous_flight_and_steering(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_OBSTACLE_AVOIDANCE:
            autonomous_obstacle_avoidance(telemetryCmd, remoteCmd);
            break;
        case ROUTE_INQUIRY:
            route_inquiry(telemetryCmd, remoteCmd);
            break;
        case SPEED_SETTING:
            speed_setting(telemetryCmd, remoteCmd);
            break;
        case ROUTE_SETTING:
            route_setting(telemetryCmd, remoteCmd);
            break;
        case DIFFERENTIAL_DATA_SETTING:
            differential_data_setting(telemetryCmd, remoteCmd);
            break;
        case ROUTE_FLIGHT_INSTRUCTIONS:
            route_flight_instructions(telemetryCmd, remoteCmd);
            break;
        case GEOGRAPHIC_COORDINATE_GUIDANCE:
            geographic_coordinage_guidance(telemetryCmd, remoteCmd);
            break;
        case ROUTE_DOWNLOAD_SWITCH:
            route_downlaod_switch(telemetryCmd, remoteCmd);
            break;
        case AUTONOMOUS_NAVIGATION_POSITIONING_SETTING:
            autonomous_navigation_positioning_setting(telemetryCmd, remoteCmd);
            break;

        case FORMATION_FLIGHT:
            formation_flight(telemetryCmd, remoteCmd);
            break;
        case FORMATION_FORMATION_TRANSFORMATION:
            formation_formation_transformation(telemetryCmd, remoteCmd);
            break;
        case ONE_CLICK_TAKEOFF_COMMAND_FOR_FORMATION:
            one_click_takeoff_command_for_formation(telemetryCmd, remoteCmd);
            break;
        case ONE_CLICK_RETURN_TO_LANDING_COMMOND_FOR_FORMATION:
            one_click_retrun_to_landing_commond_for_formation(telemetryCmd, remoteCmd);
            break;
        case NAVIGATOR_WAYPOING_SETTING:
            navigator_waypoing_setting(telemetryCmd, remoteCmd);
            break;

        /* 遥测命令子定义*/
        case IMAGE_STATUS_FEEDBACK_DATA:
            image_status_feedback_data(telemetryCmd, remoteCmd);
            break;
        case VIDEO_STATUS_FEEDBACK:
            video_status_feedback(telemetryCmd, remoteCmd);
            break;
        case LASER_RANGING_FEEDBACK:
            laser_ranging_feedback(telemetryCmd, remoteCmd);
            break;
        case LASEER_IRRADIATION_FEEDBACK:
            laseer_irradiation_feedback(telemetryCmd, remoteCmd);
            break;

        case ATTACK_PAYLOAD_FEEDBACK_DATA:
            attack_payload_feedback_data(telemetryCmd, remoteCmd);
            break;
        case ZDB_FEEDBACK_DATA:
            zdb_feedback_data(telemetryCmd, remoteCmd);
            break;

        case DRONE_PLATFORM_STATUS_FEEDBACK_DATA:
            drone_platform_status_feedback_data(telemetryCmd, remoteCmd);
            break;
        case TASK_STATUS_FEEDBACK_DATA:
            task_status_feedback_data(telemetryCmd, remoteCmd);
            break;
        case FORMATION_STATUS_FEEDBACK_DATA:
            formation_status_feedback_data(telemetryCmd, remoteCmd);
            break;

        case COMMAND_FEEDBACK_RESPONSE:
            command_feedback_response(telemetryCmd, remoteCmd);
            break;

        case ROUTE_INQUIRY_REPLY:
            route_inquiry_reply(telemetryCmd, remoteCmd);
            break;
        case ROUTE_DOWNLOAD_REPLY:
            route_download_reply(telemetryCmd, remoteCmd);
            break;
        case ROUTE_CONFIRMATION_REPLY:
            route_confirmation_reply(telemetryCmd, remoteCmd);
            break;
        default:
            invalid_cmd(telemetryCmd, remoteCmd);
            break;

    }

    bz_message_uav_up_t bz_message;
    static uint8_t count = 0;
    uint16_t crc;

    telemetryCmd->telemetry[0] = 0xA5;
    telemetryCmd->telemetry[1] = 0x5A;
    telemetryCmd->telemetry[2] = MAVLINK_MSG_ID_TELEMETRY_CMD_LEN;
    telemetryCmd->telemetry[3] = 0x01;
    telemetryCmd->telemetry[4] = count;
    telemetryCmd->telemetry[7] = 0x10;
    telemetryCmd->telemetry[8] = 0x51;

    memcpy((uint8_t*)&(bz_message.frame_header1), (uint8_t*)&(remoteCmd.telecontrol[0]), MAVLINK_MSG_ID_REMOTE_CMD_LEN);
    telemetryCmd->telemetry[5] = bz_message.receiver_sysid;
    telemetryCmd->telemetry[6] = bz_message.sender_sysid;

    // 填写每个命令的内容
    telemetryCmd->telemetry[9] = 0x01;
    telemetryCmd->telemetry[16] = 0x01;


    telemetryCmd->telemetry[10] = bz_message.cmd0;
    telemetryCmd->telemetry[11] = bz_message.cmd1;
    telemetryCmd->telemetry[12] = bz_message.sender_sysid;
    telemetryCmd->telemetry[13] = bz_message.receiver_sysid;
    telemetryCmd->telemetry[14] = 0x33;
    telemetryCmd->telemetry[15] = 0x77;

    crc = GetCRC16((uint8_t*)&telemetryCmd->telemetry[0], MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 3);
    telemetryCmd->telemetry[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 3] = crc & 0x00ff;
    telemetryCmd->telemetry[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 2] = crc >> 0x8;
    telemetryCmd->telemetry[MAVLINK_MSG_ID_TELEMETRY_CMD_LEN - 1] = 0xAA;
    count++;
}

uint16_t ProtocolConversion:: crc16_xmodem(uint8_t *data, uint16_t length)
{
    uint8_t i;
    uint16_t crc = 0;            // Initial value
    while(length--)
    {
        crc ^= (uint16_t)(*data++) << 8; // crc ^= (uint16_t)(*data)<<8; data++;
        for (i = 0; i < 8; ++i)
        {
            if ( crc & 0x8000 )
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void ProtocolConversion::print_mavlink_message(bz_message_t *bz_message)
{
    qDebug() << "print_send";
    qDebug() << "frame_header1: " << QString("0x%2").arg(bz_message->frame_header1, 0, 16);
    qDebug() << "frame_header2: "<< QString("0x%2").arg(bz_message->frame_header2, 0, 16);
    qDebug() << "frame_length: " << QString("0x%2").arg(bz_message->frame_length, 0, 16);
    qDebug() << "system_type: " << QString("0x%2").arg(bz_message->system_type, 0, 16);
    qDebug() << "seq: " << QString("0x%2").arg(bz_message->seq, 0, 16);
    qDebug() << "sender_sysid: " << QString("0x%2").arg(bz_message->sender_sysid, 0, 16);
    qDebug() << "receiver_sysid: " << QString("0x%2").arg(bz_message->receiver_sysid, 0, 16);
    qDebug() << "cmd0: " << QString("0x%2").arg(bz_message->cmd0, 0, 16);
    qDebug() << "cmd1: " << QString("0x%2").arg(bz_message->cmd1, 0, 16);
    qDebug() << "cmd: " << QString("0x%2").arg(bz_message->cmd0 << 8 | bz_message->cmd1, 0, 16);
    qDebug() << "pyload[0]: " << QString("0x%2").arg(bz_message->pyload[0], 0, 16);
    qDebug() << "ck0: " << QString("0x%2").arg(bz_message->ck0, 0, 16);
    qDebug() << "ck1: " << QString("0x%2").arg(bz_message->ck1, 0, 16);
    qDebug() << "ck: " << QString("0x%2").arg(bz_message->ck1 << 8 | bz_message->ck0, 0, 16);
    qDebug() << "frame_tail: " << QString("0x%2").arg(bz_message->frame_tail, 0, 16);
}

void ProtocolConversion::print_mavlink_message_ground_down(bz_message_ground_down_t *bz_message)
{
    qDebug() << "print_mavlink_message_ground_down";
    qDebug() << "frame_header1: " << QString("0x%2").arg(bz_message->frame_header1, 0, 16);
    qDebug() << "frame_header2: "<< QString("0x%2").arg(bz_message->frame_header2, 0, 16);
    qDebug() << "frame_length: " << QString("0x%2").arg(bz_message->frame_length, 0, 16);
    qDebug() << "system_type: " << QString("0x%2").arg(bz_message->system_type, 0, 16);
    qDebug() << "seq: " << QString("0x%2").arg(bz_message->seq, 0, 16);
    qDebug() << "sender_sysid: " << QString("0x%2").arg(bz_message->sender_sysid, 0, 16);
    qDebug() << "receiver_sysid: " << QString("0x%2").arg(bz_message->receiver_sysid, 0, 16);
    qDebug() << "cmd0: " << QString("0x%2").arg(bz_message->cmd0, 0, 16);
    qDebug() << "cmd1: " << QString("0x%2").arg(bz_message->cmd1, 0, 16);
    qDebug() << "cmd: " << QString("0x%2").arg(bz_message->cmd0 << 8 | bz_message->cmd1, 0, 16);
    print_reciver((char*)&bz_message->pyload[0], 115-12);
    qDebug() << "ck0: " << QString("0x%2").arg(bz_message->ck0, 0, 16);
    qDebug() << "ck1: " << QString("0x%2").arg(bz_message->ck1, 0, 16);
    qDebug() << "ck: " << QString("0x%2").arg(bz_message->ck1 << 8 | bz_message->ck0, 0, 16);
    qDebug() << "frame_tail: " << QString("0x%2").arg(bz_message->frame_tail, 0, 16);
}
void ProtocolConversion::print_reciver(char *buff, uint8_t len)
{
    QByteArray print_array(buff, len);
    qDebug() << "print_reciver: " << print_array.toHex();
}

void ProtocolConversion::print_send(QByteArray data)
{
    bz_message_ground_down_t *bz_message;
    char buff[115] = {0};
    drone_platform_status_feedback_data_t feedback_data = {0};

    uint8_t len = data.size();
    memcpy(buff, data.data(), len);
    memcpy((char*)&feedback_data.flight_control_version, (char*)&buff[9], 63-9+1);

    bz_message = (bz_message_ground_down_t *)buff;
    print_mavlink_message_ground_down(bz_message);

    qDebug("feedback_data.flight_control_version: 0x%x", feedback_data.flight_control_version);
    qDebug("feedback_data.hw_version: 0x%x", feedback_data.hw_version);
    qDebug("feedback_data.longitude: 0x%x", feedback_data.longitude);
    qDebug("feedback_data.latitude: 0x%x", feedback_data.latitude);
    qDebug("feedback_data.relative_height: 0x%x", feedback_data.relative_height);
    qDebug("feedback_data.yaw_angle: 0x%x", feedback_data.yaw_angle);
    qDebug("feedback_data.pitch_angle: 0x%x", feedback_data.pitch_angle);
    qDebug("feedback_data.roll_angle: 0x%x", feedback_data.roll_angle);
    qDebug("feedback_data.northbound_velocity: 0x%x", feedback_data.northbound_velocity);
    qDebug("feedback_data.westbound_velocity: 0x%x", feedback_data.westbound_velocity);
    qDebug("feedback_data.vertical_velocity: 0x%x", feedback_data.vertical_velocity);
    qDebug("feedback_data.ground_speed: 0x%x", feedback_data.ground_speed);
    qDebug("feedback_data.battery_voltage: 0x%x", feedback_data.battery_voltage);
    qDebug("feedback_data.percentage_of_remaining_battery_power: 0x%x", feedback_data.percentage_of_remaining_battery_power);
    qDebug("feedback_data.flight_status: 0x%x", feedback_data.flight_status);
    qDebug("feedback_data.satellite_positioning_mode: 0x%x", feedback_data.satellite_positioning_mode);
    qDebug("feedback_data.loss_satellite_time: 0x%x", feedback_data.loss_satellite_time);
    qDebug("feedback_data.airline_bus: 0x%x", feedback_data.airline_bus);
    qDebug("feedback_data.total_number_of_routes: 0x%x", feedback_data.total_number_of_routes);
    qDebug("feedback_data.current_route_number: 0x%x", feedback_data.current_route_number);
    qDebug("feedback_data.total_number_of_waypoints: 0x%x", feedback_data.total_number_of_waypoints);
    qDebug("feedback_data.current_waypoint_number: 0x%x", feedback_data.current_waypoint_number);
    qDebug("feedback_data.longitude_of_the_next_waypoint: 0x%x", feedback_data.longitude_of_the_next_waypoint);
    qDebug("feedback_data.latitude_of_the_next_waypoint: 0x%x", feedback_data.latitude_of_the_next_waypoint);
    qDebug("feedback_data.relative_altitude_of_the_next_waypoint: 0x%x", feedback_data.relative_altitude_of_the_next_waypoint);
    qDebug("feedback_data.autonomous_obstacle_avoidance_state: 0x%x", feedback_data.autonomous_obstacle_avoidance_state);
    qDebug("feedback_data.platform_status: 0x%x", feedback_data.platform_status);
    qDebug("feedback_data.flight_time: 0x%x", feedback_data.flight_time);
    qDebug("feedback_data.startup_time: 0x%x", feedback_data.startup_time);
    qDebug("feedback_data.flight_confirmation: 0x%x", feedback_data.flight_confirmation);


}

void ProtocolConversion::qbyte_array_to_char(uint8_t *buff, int *len, QByteArray array)
{
    memcpy((uint8_t*)buff, (uint8_t*)array.data(), *len);
}

void ProtocolConversion::char_to_qbyte_array(QByteArray *array, char* buff, int len)
{
    memcpy(array->data(), buff, len);
}


void ProtocolConversion::work_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************work_mode**************************************";
}

void ProtocolConversion::light_switching(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************light_switching**************************************";
}
void ProtocolConversion::image_stabilization_instruction(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************image_stabilization_instruction**************************************";
}

void ProtocolConversion::zoom(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************zoom**************************************";
}

void ProtocolConversion::laser_irradiattion(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************laser_irradiattion**************************************";
}

void ProtocolConversion::photography(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************photography**************************************";
}

void ProtocolConversion::centering(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************centering**************************************";
}
void ProtocolConversion::video(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************video**************************************";
}

void ProtocolConversion::target_detection_and_recognition(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************target_detection_and_recognition**************************************";
}

void ProtocolConversion::automatic_target_tracking(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************automatic_target_tracking**************************************";
}

void ProtocolConversion::pan_tilt_control_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************pan_tilt_control_mode**************************************";
}

void ProtocolConversion::optoelectronic_load_status_query(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************optoelectronic_load_status_query**************************************";
}


void ProtocolConversion::GJ_work_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************GJ_work_mode**************************************";
}

void ProtocolConversion::attack_mode_switching(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************attack_mode_switching**************************************";
}

void ProtocolConversion::insurance_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************insurance_mode**************************************";
}

void ProtocolConversion::attack_status(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************attack_status**************************************";
}

void ProtocolConversion::GJ_automatic_target_tracking(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************GJ_automatic_target_tracking**************************************";
}

void ProtocolConversion::autonomous_strike(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_strike**************************************";
}

void ProtocolConversion::GJ_load_status_query(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************GJ_load_status_query**************************************";
}

void ProtocolConversion::ZDB_control(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************ZDB_control**************************************";
}


void ProtocolConversion::virtual_rocker_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************virtual_rocker_mode**************************************";
}

void ProtocolConversion::autonomous_takeoff(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_takeoff**************************************";
}

void ProtocolConversion::autonomous_return(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_return**************************************";
}

void ProtocolConversion::autonomous_cruise(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_cruise**************************************";
}

void ProtocolConversion::autonomous_flight_and_steering(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_flight_and_steering**************************************";
}

void ProtocolConversion::autonomous_obstacle_avoidance(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_obstacle_avoidance**************************************";
}

void ProtocolConversion::route_inquiry(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_inquiry**************************************";
}

void ProtocolConversion::speed_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************speed_setting**************************************";
}

void ProtocolConversion::route_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_setting**************************************";
}

void ProtocolConversion::differential_data_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************differential_data_setting**************************************";
}

void ProtocolConversion::route_flight_instructions(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_flight_instructions**************************************";
}

void ProtocolConversion::geographic_coordinage_guidance(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************geographic_coordinage_guidance**************************************";
}

void ProtocolConversion::route_downlaod_switch(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_downlaod_switch**************************************";
}

void ProtocolConversion::autonomous_navigation_positioning_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************autonomous_navigation_positioning_setting**************************************";
}


void ProtocolConversion::formation_flight(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************formation_flight**************************************";
}

void ProtocolConversion::formation_formation_transformation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************formation_formation_transformation**************************************";
}

void ProtocolConversion::one_click_takeoff_command_for_formation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************one_click_takeoff_command_for_formation**************************************";
}

void ProtocolConversion::one_click_retrun_to_landing_commond_for_formation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************one_click_retrun_to_landing_commond_for_formation**************************************";
}

void ProtocolConversion::navigator_waypoing_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************navigator_waypoing_setting**************************************";
}



void ProtocolConversion::image_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************image_status_feedback_data**************************************";
}

void ProtocolConversion::video_status_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************video_status_feedback**************************************";
}

void ProtocolConversion::laser_ranging_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************laser_ranging_feedback**************************************";
}

void ProtocolConversion::laseer_irradiation_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************laseer_irradiation_feedback**************************************";
}


void ProtocolConversion::attack_payload_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************attack_payload_feedback_data**************************************";
}

void ProtocolConversion::zdb_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************zdb_feedback_data**************************************";
}


void ProtocolConversion::drone_platform_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************drone_platform_status_feedback_data**************************************";
}

void ProtocolConversion::task_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************task_status_feedback_data**************************************";
}

void ProtocolConversion::formation_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************formation_status_feedback_data**************************************";
}


void ProtocolConversion::command_feedback_response(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************command_feedback_response**************************************";
}


void ProtocolConversion::route_inquiry_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_inquiry_reply**************************************";
}

void ProtocolConversion::route_download_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_download_reply**************************************";
}

void ProtocolConversion::route_confirmation_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug() << "***************************************route_confirmation_reply**************************************";
}

void ProtocolConversion::invalid_cmd(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd)
{
    qDebug("***************************************invalid_cmd : 0x%02x**************************************", ((remoteCmd.telecontrol[7] << 8) | remoteCmd.telecontrol[8]));
}
