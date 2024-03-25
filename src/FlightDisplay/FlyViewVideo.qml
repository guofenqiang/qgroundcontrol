/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick 2.12
import QtQuick.Controls 2.15

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0

import PodController                1.0

Item {
    id:         _root
    visible:    QGroundControl.videoManager.hasVideo

    property Item pipState: videoPipState
    property point dragStartPos: Qt.point(0, 0)
    property bool isDragging: false
    QGCPipState {
        id:         videoPipState
        pipOverlay: _pipOverlay
        isDark:     true

        onWindowAboutToOpen: {
            QGroundControl.videoManager.stopVideo()
            videoStartDelay.start()
        }

        onWindowAboutToClose: {
            QGroundControl.videoManager.stopVideo()
            videoStartDelay.start()
        }

        onStateChanged: {
            if (pipState.state !== pipState.fullState) {
                QGroundControl.videoManager.fullScreen = false
            }
        }
    }

    Timer {
        id:           videoStartDelay
        interval:     2000;
        running:      false
        repeat:       false
        onTriggered:  QGroundControl.videoManager.startVideo()
    }

    //-- Video Streaming
    FlightDisplayViewVideo {
        id:             videoStreaming
        anchors.fill:   parent
        useSmallFont:   _root.pipState.state !== _root.pipState.fullState
        visible:        QGroundControl.videoManager.isGStreamer
    }
    //-- UVC Video (USB Camera or Video Device)
    Loader {
        id:             cameraLoader
        anchors.fill:   parent
        visible:        !QGroundControl.videoManager.isGStreamer
        source:         QGroundControl.videoManager.uvcEnabled ? "qrc:/qml/FlightDisplayViewUVC.qml" : "qrc:/qml/FlightDisplayViewDummy.qml"
    }

    QGCLabel {
        text: qsTr("Double-click to exit full screen")
        font.pointSize: ScreenTools.largeFontPointSize
        visible: QGroundControl.videoManager.fullScreen && flyViewVideoMouseArea.containsMouse
        anchors.centerIn: parent

        onVisibleChanged: {
            if (visible) {
                labelAnimation.start()
            }
        }

        PropertyAnimation on opacity {
            id: labelAnimation
            duration: 10000
            from: 1.0
            to: 0.0
            easing.type: Easing.InExpo
        }
    }

    MouseArea {
        id: flyViewVideoMouseArea
        anchors.fill:       parent
        enabled:            pipState.state === pipState.fullState
        hoverEnabled: true
        onDoubleClicked:    QGroundControl.videoManager.fullScreen = !QGroundControl.videoManager.fullScreen
    }

    ProximityRadarVideoView{
        anchors.fill:   parent
        vehicle:        QGroundControl.multiVehicleManager.activeVehicle
    }

    ObstacleDistanceOverlayVideo {
        id: obstacleDistance
        showText: pipState.state === pipState.fullState
    }

    MouseArea {
        anchors.fill: parent

        onWheel: {
            wheel.accepted = true;
            pod_controller_id.mauanlFocus(wheel.angleDelta.x, wheel.angleDelta.y)
        }

        onPressed: {
            dragStartPos = Qt.point(mouse.x, mouse.y)
            isDragging = true
        }

        onPositionChanged: {
            if (isDragging) {
                var delta = Qt.point(mouse.x - dragStartPos.x, mouse.y - dragStartPos.y)
                pod_controller_id.sendRotateCommand(delta.x, delta.y)
            }
        }

        onReleased: {
            isDragging = false
            pod_controller_id.sendRotateCommand(0, 0)
        }
    }

    Rectangle {
        id: customComponent
        x: ScreenTools.defaultFontPixelWidth * 10
        anchors.top: parent.top
        width: ScreenTools.defaultFontPixelWidth * 8
        height: ScreenTools.defaultFontPixelHeight
        color: "lightblue"

        Text {
            anchors.centerIn: parent
            text: "一键回中"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pod_controller_id.oneClickReturnToCenter()
            }
        }
    }

    PodController {
        id: pod_controller_id
        objectName: "pod_controller_obj"
        Component.onCompleted: {
            console.log("init pod_controller")
        }
    }
}
