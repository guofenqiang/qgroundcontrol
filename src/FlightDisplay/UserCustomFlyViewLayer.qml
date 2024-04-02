import QtQuick 2.11
import QGroundControl               1.0
import Telecontrol                  1.0
import Ecm                          1.0
import TcpServer                    1.0
import UAVEcm                       1.0
import MavlinkPod                   1.0

Item {
    id: _root
    anchors.fill: parent

    property var    parentToolInsets
    property var    mapControl
    property real   _controlX
    property real   _controlY
    property real   _controlWidth
    property real   _controlHeight

    onWidthChanged: {
        _calPosition()
    }

    onHeightChanged: {
        _calPosition()
    }

    function _calPosition() {
        if (parentToolInsets.pipState.state === parentToolInsets.pipState.pipState) {
            _controlX = parentToolInsets.width + _toolsMargin
            _controlY = _root.height - parentToolInsets.height - _toolsMargin
            _controlWidth = _root.width - parentToolInsets.width - _toolsMargin
            _controlHeight = parentToolInsets.height
        } else if (mapControl.pipState.state === mapControl.pipState.pipState) {
            _controlX = mapControl.width + _toolsMargin
            _controlY = _root.height - mapControl.height - _toolsMargin
            _controlWidth = _root.width - mapControl.width - _toolsMargin
            _controlHeight = mapControl.height
        }
    }

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

    Rectangle{
        id: userData
        x: _controlX
        y: _controlY
        z: QGroundControl.zOrderTopMost
        width: _controlWidth
        height: _controlHeight
        anchors.margins: _margins * 2
        visible: true

        UserFlyViewDisplayControlLayer {

        }
    }
}
