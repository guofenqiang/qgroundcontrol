import QtQuick 2.11
import QGroundControl               1.0
import Telecontrol                  1.0
import Ecm                          1.0
import TcpServer                    1.0
import UAVEcm                       1.0
import MavlinkPod                   1.0

Item {
    // 仅为了在qml初始化Telecontrol类
    Telecontrol {
      id: telecontrol
      objectName: "telecontrol"
      Component.onCompleted: {
          console.log("init telecontrol")
      }
    }

    Ecm {
      id: ecm_id
      objectName: "ecm_obj"
      Component.onCompleted: {
          console.log("init ecm")
      }
    }

    TcpServer {
        id: tcp_server_id
        objectName: "tcp_server_obj"
        Component.onCompleted: {
            console.log("init tcp_server")
        }
    }

    UAVEcm {
        id: uav_ecm_id
        objectName: "uav_ecm_obj"
        Component.onCompleted: {
            console.log("init uav_ecm")
        }
    }

    MavlinkPod {
        id: mavlink_pod_id
        objectName: "mavlink_pod_obj"
        Component.onCompleted: {
            console.log("init mavlik_pod")
        }
    }

    TelemetryData {
        id: telemetryData
    }

    EcmActionLayer {
        id: ecmActionLayer
    }
}
