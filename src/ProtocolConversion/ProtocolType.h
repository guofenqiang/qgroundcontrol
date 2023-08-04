#ifndef PROTOCOLTYPE_H
#define PROTOCOLTYPE_H

typedef struct __bz_message_t {
    uint8_t frame_header1;
    uint8_t frame_header2;
    uint8_t frame_length;
    uint8_t system_type;
    uint8_t seq;
    uint8_t sender_sysid;
    uint8_t receiver_sysid;
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t pyload[128 - 12];
    uint8_t ck0;
    uint8_t ck1;
    uint8_t frame_tail;
} bz_message_t; //bz_message_uav_down_t

typedef struct __bz_message_uav_up_t {
    uint8_t frame_header1;
    uint8_t frame_header2;
    uint8_t frame_length;
    uint8_t system_type;
    uint8_t seq;
    uint8_t sender_sysid;
    uint8_t receiver_sysid;
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t pyload[64 - 12];
    uint8_t ck0;
    uint8_t ck1;
    uint8_t frame_tail;
} bz_message_uav_up_t;

typedef struct __bz_message_ground_down_t {
    uint8_t frame_header1;
    uint8_t frame_header2;
    uint8_t frame_length;
    uint8_t system_type;
    uint8_t seq;
    uint8_t sender_sysid;
    uint8_t receiver_sysid;
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t pyload[115 - 12];
    uint8_t ck0;
    uint8_t ck1;
    uint8_t frame_tail;
} bz_message_ground_down_t;

typedef struct __bz_message_ground_up_t {
    uint8_t frame_header1;
    uint8_t frame_header2;
    uint8_t frame_length;
    uint8_t system_type;
    uint8_t seq;
    uint8_t sender_sysid;
    uint8_t receiver_sysid;
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t pyload[53 - 12];
    uint8_t ck0;
    uint8_t ck1;
    uint8_t frame_tail;
} bz_message_ground_up_t;

//typedef struct __remote_cmd_t {
//    uint8_t telecontrol[64];
//} remote_cmd_t;

//typedef struct __telemetry_cmd_t {
//    uint8_t telemetry[128];
//} telemetry_cmd_t;

/* 遥控命令子定义*/
enum class TelecontrolPhotoelectricLoad {
    WORK_MODE = 0x0001,
    LIGHT_SWITCHING,
    IMAGE_STABILIZATION_INSTRUCTION,
    ZOOM,
    LASER_IRRADIATION,
    PHOTOGRAPHY,
    CENTERING,
    VIDEO,
    TARGET_DETECTION_AND_RECOGNITION,
    AUTOMATIC_TARGET_TRACKING,
//    GEOGRAPHIC_COORDINATE_GUIDANCE, //该指令弃用
    PAN_TILT_CONTROL_MODE = 0X000C,
    OPTOELECTRONIC_LOAD_STATUS_QUERY,
};

enum class TelecontrolGJLoad {
    WORK_MODE = 0x0021,
    ATTACK_MODE_SWITCHING,
    INSURANCE_MODE,
    ATTACK_STATUS,
    AUTOMATIC_TARGET_TRACKING,
    AUTONOMOUS_STRIKE,
    GJ_LOAD_STATUS_QUERY,
    ZDB_CONTROL,
};

enum class TelecontrolDronePlatform {
    VIRTUAL_ROCKER_MODE = 0x00A1,
    AUTONOMOUS_TAKEOFF,
    AUTONOMOUS_RETURN,
    AUTONOMOUS_CRUISE,
    AUTONOMOUS_FLIGHT_AND_STEERING,
    AUTONOMOUS_OBSTACLE_AVOIDANCE,
    ROUTE_INQUIRY,
    SPEED_SETTING,
    ROUTE_SETTING,
    DIFFERENTIAL_DATA_SETTING,
    ROUTE_FLIGHT_INSTRUCTIONS,
    GEOGRAPHIC_COORDINATE_GUIDANCE,
    ROUTE_DOWNLOAD_SWITCH,
    AUTONOMOUS_NAVIGATION_POSITIONING_SETTING,
};

enum class TelecontrolFormationFlight {
    FORMATION_FLIGHT = 0x00D1,
    FORMATION_FORMATION_TRANSFORMATION,
    ONE_CLICK_TAKEOFF_COMMAND_FOR_FORMATION,
    ONE_CLICK_RETURN_TO_LANDING_COMMOND_FOR_FORMATION,
    NAVIGATOR_WAYPOING_SETTING
};

/* 遥测命令子定义*/
enum class TelemetryPhotoelectricLoad {
    IMAGE_STATUS_FEEDBACK_DATA = 0x1001,
    VIDEO_STATUS_FEEDBACK,
    LASER_RANGING_FEEDBACK,
    LASEER_IRRADIATION_FEEDBACK,
};

enum class TelemetryGJLoad {
    ATTACK_PAYLOAD_FEEDBACK_DATA = 0x1021,
    ZDB_FEEDBACK_DATA,
};

// 每200ms发送一次，定时更新，类似心跳包
enum class TelemetryDronePlatform {
    DRONE_PLATFORM_STATUS_FEEDBACK_DATA = 0x1031,
    TASK_STATUS_FEEDBACK_DATA,
    FORMATION_STATUS_FEEDBACK_DATA,
};

enum class TelemetryInstructionFeedback {
    COMMAND_FEEDBACK_RESPONSE = 0x1051, //每收到一次遥控指令都需发送一次指令反馈回复（0X1051）
};

enum class TelemetryRouteManagement {
    ROUTE_INQUIRY_REPLY = 0x1052,
    ROUTE_DOWNLOAD_REPLY,
    ROUTE_CONFIRMATION_REPLY,
};

typedef struct __drone_platform_status_feedback_data {
    uint8_t flight_control_version;
    uint8_t hw_version;
    int32_t longitude;
    int32_t latitude;
    int16_t relative_height;
    int16_t yaw_angle; //偏航角
    int16_t pitch_angle; //俯仰角
    int16_t roll_angle; //横滚角
    int16_t northbound_velocity;
    int16_t westbound_velocity;
    int16_t vertical_velocity;
    uint16_t ground_speed;
    uint16_t battery_voltage;
    uint8_t percentage_of_remaining_battery_power;
    uint8_t flight_status;
    uint8_t satellite_positioning_mode;
    uint8_t loss_satellite_time;
    uint16_t airline_bus;
    uint8_t	total_number_of_routes;
    uint8_t	current_route_number;
    uint8_t	total_number_of_waypoints;
    uint8_t	current_waypoint_number;
    int32_t	longitude_of_the_next_waypoint;
    int32_t	latitude_of_the_next_waypoint;
    int16_t	relative_altitude_of_the_next_waypoint;
    uint8_t	autonomous_obstacle_avoidance_state;
    uint8_t	platform_status;
    uint16_t flight_time;
    uint16_t startup_time;
    uint8_t	flight_confirmation;
} drone_platform_status_feedback_data_t;

#endif // PROTOCOLTYPE_H
