#ifndef PROTOCOLCONVERSION_H
#define PROTOCOLCONVERSION_H

#include <QObject>
#include "QByteArray"
#include "ProtocolType.h"
#include "mavlink.h"

class ProtocolConversion : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolConversion(QObject *parent = nullptr);

    void bz_protocol_conversion(QByteArray buff, mavlink_remote_cmd_t* remote_cmd);
    void CmdFeedback(mavlink_telemetry_cmd_t telemetry_cmd, QByteArray &buff);
    uint16_t crc16_xmodem(uint8_t *data, uint16_t length);
    void bz_encode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remote_cmd);
    void print_mavlink_message(bz_message_t *bz_message);
    void print_mavlink_message_ground_down(bz_message_ground_down_t *bz_message);
    void print_reciver(char *buff, uint8_t len);
    void print_send(QByteArray data);
    void qbyte_array_to_char(uint8_t *buff, int *len, QByteArray array);
    void char_to_qbyte_array(QByteArray *array, char* buff, int len);

    void work_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void light_switching(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void image_stabilization_instruction(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void zoom(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void laser_irradiattion(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void photography(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void centering(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void video(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void target_detection_and_recognition(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void automatic_target_tracking(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void pan_tilt_control_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void optoelectronic_load_status_query(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void GJ_work_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void attack_mode_switching(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void insurance_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void attack_status(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void GJ_automatic_target_tracking(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_strike(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void GJ_load_status_query(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void ZDB_control(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void virtual_rocker_mode(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_takeoff(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_return(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_cruise(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_flight_and_steering(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_obstacle_avoidance(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_inquiry(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void speed_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void differential_data_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_flight_instructions(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void geographic_coordinage_guidance(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_downlaod_switch(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void autonomous_navigation_positioning_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void formation_flight(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void formation_formation_transformation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void one_click_takeoff_command_for_formation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void one_click_retrun_to_landing_commond_for_formation(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void navigator_waypoing_setting(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);


    void image_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void video_status_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void laser_ranging_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void laseer_irradiation_feedback(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void attack_payload_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void zdb_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void drone_platform_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void task_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void formation_status_feedback_data(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void command_feedback_response(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

    void route_inquiry_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_download_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void route_confirmation_reply(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);
    void invalid_cmd(mavlink_telemetry_cmd_t *telemetryCmd,  mavlink_remote_cmd_t remoteCmd);

signals:

};

#endif // PROTOCOLCONVERSION_H
