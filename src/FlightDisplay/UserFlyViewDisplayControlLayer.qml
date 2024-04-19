import QtQuick 2.11
import QtQuick.Controls             2.5
import QtQuick.Layouts              1.15

import QGroundControl                       1.0
import QGroundControl.FactSystem            1.0
import QGroundControl.FactControls          1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import CustomTools.BasicCompent             1.0

Rectangle {
    id: outRect
    anchors.fill: parent
    color: 'blue'
    property var    vehicle:              QGroundControl.multiVehicleManager.activeVehicle
    property string flightMode:           vehicle ? vehicle.flightMode : '0'
    property real   roll:                 vehicle ? vehicle.roll.rawValue: 0
    property real   groundSpeed:          vehicle ? vehicle.groundSpeed.rawValue: 0
    property real   airSpeed:             vehicle ? vehicle.airSpeed.rawValue: 0
    property real   climbRate:            vehicle ? vehicle.climbRate.rawValue: 0
    property real   altitudeRelative:     vehicle ? vehicle.altitudeRelative.rawValue: 0
    property real   flightDistance:       vehicle ? vehicle.flightDistance.rawValue: 0
    property real   distanceToHome:       vehicle ? vehicle.distanceToHome.rawValue: 0
    property real   pitchAngle:             vehicle ? vehicle.pitch.rawValue : 0
    property real   yawAngle:               vehicle ? vehicle.yawRate.rawValue : 0
    property real   _heading:               vehicle ? vehicle.heading.rawValue : 0
    property real   _latitude:              vehicle ? vehicle.latitude : 0
    property real   _longitude:             vehicle ? vehicle.longitude : 0
    property string _version:               vehicle ? vehicle.firmwareMajorVersion + "." + vehicle.firmwareMinorVersion + "." + vehicle.firmwarePatchVersion + vehicle.firmwareVersionTypeString : qsTr("Unknown")
    property real   _flightTime:            vehicle ? vehicle.flightTime.rawValue : 0
    property real   _guidedMinimumAltitude: QGroundControl.settingsManager.flyViewSettings.guidedMinimumAltitude.rawValue

    property var    _batteryGroup:          globals.activeVehicle && globals.activeVehicle.batteries.count ? globals.activeVehicle.batteries.get(0) : undefined
    property var    _batteryValue:          _batteryGroup ? _batteryGroup.percentRemaining.value : 0
    property var    _batPercentRemaining:   isNaN(_batteryValue) ? 0 : _batteryValue

    property string  _fixBackgroundColor:       "#2B3042"
    property string  _fixBorderColor:           "#39FF14"
    property string  _fixTextColor:             "#39FF14"
    property int     _fixRadius:                10

    property string _sideBackgroundColor:       "#2B3042"
    property string _sideBorderColor:           "#00FFFF"
    property string _sideTextColor:             "#00FFFF"
    property int    _sideRadius:                10

    Rectangle {
        id: innerRect
        anchors.fill: parent
        anchors.margins: 2

        Row {
            id: userView
            anchors.fill: parent
            property real leftRectWidthRatio: 8 / 20
            property real fixTelemetryWidthRatio: 4 / 20
            property real rightRationWidthRatio: 8 / 20
            /* 本机编队和飞行 */
            Rectangle {
                id: leftRect
                width: parent.width * userView.leftRectWidthRatio
                height: parent.height

                TabBar {
                    id: leftBar
                    width: firstBtn.width
                    TabButton {
                        id: firstBtn
                        text: qsTr("本机")
                        width: leftRect.width/8
                        height: leftRect.height/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                    }
                    TabButton {
                        id: secondBtn
                        text: qsTr("编队")
                        width: leftRect.width/8
                        height: leftRect.height/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: firstBtn.bottom
                    }
                }

                StackLayout {
                    id: view

                    currentIndex: leftBar.currentIndex
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: leftBar.right
                    anchors.right: parent.right
                    anchors.margins: 2

                    /*本机*/
                    Rectangle {
                        id: localUAV
                        property int avgWidth: parent.width / 2
                        property int avgHeight: parent.height / 5
                        Grid {
                            anchors.fill: parent
                            columns: 2
                            spacing: 1

                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "飞控版本: " + _version
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "平台版本: " + QGroundControl.qgcVersion.substring(0, 6)
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "横滚: " + roll.toFixed(7)
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "俯仰: " + pitchAngle.toFixed(7)
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "偏航角度: " + _heading.toFixed(1)
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: "飞行时间: " + _flightTime.toFixed(0) + " s"
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight;
                                Row {
                                    anchors.fill: parent
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomText {
                                            anchors.centerIn: parent
                                            text: "起飞高度"
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomComboBox {
                                            width: parent.width
                                            height: parent.height
                                            anchors.centerIn: parent
                                            model: [1, 2, 3, 4, 5, 10, 15, 20]

                                            onActivated: {
                                                QGroundControl.settingsManager.flyViewSettings.guidedMinimumAltitude.rawValue = model[index]
                                            }

                                            Component.onCompleted: currentIndex = indexOfValue(_guidedMinimumAltitude)
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight;
                                Row {
                                    anchors.fill: parent
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomText {
                                            anchors.centerIn: parent
                                            text: "飞行模式"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomComboBox {
                                            width: parent.width
                                            height: parent.height
                                            anchors.centerIn: parent
                                            model: ["Stabiled", "Altitude", "Position"]

                                            onActivated: {
                                                vehicle.flightMode = model[index]
                                            }
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: qsTr('维度: ') + _latitude.toFixed(7)
                                }
                            }
                            Rectangle {
                                width: localUAV.avgWidth ; height: localUAV.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: qsTr('经度: ') + _longitude.toFixed(7)
                                }
                            }
                        }
                    }

                    /*编队*/
                    Rectangle {
                        id: formationID
                        property real avgWidth: width / 2
                        property real avgHeight: height / 5.1
                        Grid {
                            anchors.fill: parent
                            columns: 2
                            spacing: 1
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: '目标维度: 0'
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomText {
                                    anchors.centerIn: parent
                                    text: '目标经度: 0'
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight
                                Row {
                                    anchors.fill: parent
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomText {
                                            anchors.centerIn: parent
                                            text: "目标高度"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomComboBox {
                                            width: parent.width
                                            height: parent.height
                                            anchors.centerIn: parent
                                            model: [5, 10, 15, 20, 30, 40, 50]
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight
                                Row {
                                    anchors.fill: parent
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomText {
                                            anchors.centerIn: parent
                                            text: "领航者ID"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 2
                                        height: parent.height
                                        color: outRect._sideBackgroundColor
                                        border.color: outRect._sideBorderColor
                                        radius: outRect._sideRadius
                                        CustomComboBox {
                                            width: parent.width
                                            height: parent.height
                                            anchors.centerIn: parent
                                            model: 8
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight; border.color: outRect._sideBorderColor
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomComboBox {
                                    model: ["维持原队形", "一字型队形", "人字形队形", "圆形队形"]
                                    width: parent.width
                                    height: parent.height
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight;
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomButton {
                                    width: parent.width
                                    height: parent.height
                                    text: "编队飞行"
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight;
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomButton {
                                    width: parent.width
                                    height: parent.height
                                    text: "一键起飞"
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight;
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomButton {
                                    width: parent.width
                                    height: parent.height
                                    text: "一键返航"
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight;
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomButton {
                                    width: parent.width
                                    height: parent.height
                                    text: "采集坐标"
                                }
                            }
                            Rectangle {
                                width: formationID.avgWidth; height: formationID.avgHeight;
                                color: outRect._sideBackgroundColor
                                radius: outRect._sideRadius
                                CustomButton {
                                    width: parent.width
                                    height: parent.height
                                    text: "航点设置"
                                }
                            }
                        }
                    }
                }
            }
            /* 固定显示遥测栏*/
            Rectangle {
                id: fixTelemetry
                width: parent.width * userView.fixTelemetryWidthRatio
                height: parent.height
                anchors.margins: 20

                property real avgHeight: parent.height / 5.1

                Column {
                    anchors.fill: parent
                    spacing: 1

                    Rectangle {
                        width: parent.width; height: fixTelemetry.avgHeight; border.color: outRect._fixBorderColor
                        color: outRect._fixBackgroundColor
                        radius: outRect._fixRadius
                        CustomText {
                            anchors.centerIn: parent
                            text: "相对高度 " + altitudeRelative.toFixed(1) + " m"
                            fontColor: outRect._fixTextColor
                        }
                    }
                    Rectangle {
                        width: parent.width; height: fixTelemetry.avgHeight; border.color: outRect._fixBorderColor
                        color: outRect._fixBackgroundColor
                        radius: outRect._fixRadius
                        CustomText {
                            anchors.centerIn: parent
                            text: "爬升速度 " + climbRate.toFixed(1) + " m/s"
                            fontColor: outRect._fixTextColor
                        }
                    }
                    Rectangle {
                        width: parent.width; height: fixTelemetry.avgHeight; border.color: outRect._fixBorderColor
                        color: outRect._fixBackgroundColor
                        radius: outRect._fixRadius
                        CustomText {
                            anchors.centerIn: parent
                            text: "到home点距离 " + distanceToHome.toFixed(1) + " m"
                            fontColor: outRect._fixTextColor
                        }
                    }
                    Rectangle {
                        width: parent.width; height: fixTelemetry.avgHeight; border.color: outRect._fixBorderColor
                        color: outRect._fixBackgroundColor
                        radius: outRect._fixRadius
                        CustomText {
                            anchors.centerIn: parent
                            text: "地速 " + groundSpeed.toFixed(1) + " m/s"
                            fontColor: outRect._fixTextColor
                        }
                    }
                    Rectangle {
                        width: parent.width; height: fixTelemetry.avgHeight; border.color: outRect._fixBorderColor
                        color: outRect._fixBackgroundColor
                        radius: outRect._fixRadius
                        CustomText {
                            anchors.centerIn: parent
                            text: "飞行距离 " + flightDistance.toFixed(1) + " m"
                            fontColor: outRect._fixTextColor
                        }
                    }
                }
            }
            /* 侦察和处置 */
            Rectangle {
                id: rightRect
                width: parent.width * userView.rightRationWidthRatio
                height: parent.height
                border.color: "black"

                TabBar {
                    id: rightBar
                    x: parent.width - rightFirstBtn.width
                    width: rightFirstBtn.width
                    TabButton {
                        id: rightFirstBtn
                        text: qsTr("侦察")
                        width: rightRect.width/8
                        height: rightRect.height/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: parent.top
                    }
                    TabButton {
                        id: rightSecondBtn
                        text: qsTr("处置")
                        width: rightRect.width/8
                        height: rightRect.height/2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: rightFirstBtn.bottom
                    }
                }

                StackLayout {
                    id: viewRight

                    currentIndex: rightBar.currentIndex
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: rightBar.left
                    anchors.margins: 2

                    /* 侦察 */
                    Rectangle {
                        id: scoutID
                        Column {
                            anchors.fill: parent
                            spacing: 1
                            Rectangle {
                                width: parent.width; height: parent.height * 2 / 5.1
                                border.color: "black"
                                border.width: 2
                                Column {
                                    anchors.fill: parent
                                    anchors.margins: 2
                                    Rectangle {
                                        width: parent.width; height: parent.height / 2
                                        CustomRangeSlider {
                                            id: rangeSlider
                                            anchors.fill: parent
                                            from:  800
                                            to: 6000
                                            first.value: 800
                                            second.value: 6000
                                            stepSize: 1
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width; height: parent.height / 2
                                        Row {
                                            anchors.fill: parent
                                            anchors.margins: 2
                                            Rectangle {
                                                width: parent.width; height: parent.height
                                                Row {
                                                    anchors.fill: parent
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height
                                                        CustomButton {
                                                            width: parent.width
                                                            height: parent.height
                                                            text: "-"
                                                            onClicked: {
                                                                rangeSlider.first.decrease()
                                                            }
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height
                                                        CustomButton {
                                                            width: parent.width
                                                            height: parent.height
                                                            text: "+"
                                                            onClicked: {
                                                                rangeSlider.first.increase()
                                                            }
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 2 / 8; height: parent.height; border.color: outRect._sideBorderColor
                                                        color: outRect._sideBackgroundColor
                                                        radius: outRect._sideRadius
                                                        CustomText {
                                                            anchors.centerIn: parent
                                                            text: "侦察频段:"
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height; border.color: outRect._sideBorderColor
                                                        color: outRect._sideBackgroundColor
                                                        radius: outRect._sideRadius
                                                        CustomText {
                                                            anchors.fill: parent
                                                            anchors.centerIn: parent
                                                            text: rangeSlider.first.value.toFixed(0)
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height; border.color: outRect._sideBorderColor
                                                        color: outRect._sideBackgroundColor
                                                        radius: outRect._sideRadius
                                                        CustomText {
                                                            anchors.fill: parent
                                                            anchors.centerIn: parent
                                                            text: rangeSlider.second.value.toFixed(0)
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height
                                                        CustomButton {
                                                            width: parent.width
                                                            height: parent.height
                                                            text: "-"
                                                            onClicked: {
                                                                rangeSlider.second.decrease()
                                                            }
                                                        }
                                                    }
                                                    Rectangle {
                                                        width: parent.width * 1 / 8; height: parent.height
                                                        CustomButton {
                                                            width: parent.width
                                                            height: parent.height
                                                            text: "+"
                                                            onClicked: {
                                                                rangeSlider.second.increase()
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                width: parent.width; height: parent.height * 2 / 5.1
                                border.color: "black"
                                border.width: 2
                                Column {
                                    anchors.fill: parent
                                    anchors.margins: 2
                                    Rectangle {
                                        width: parent.width; height: parent.height / 2
                                        CustomSlider {
                                            id: slider
                                            anchors.fill: parent
                                            from: 800
                                            to: 6000
                                            value: 800
                                            stepSize: 1
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width; height: parent.height / 2
                                        Row {
                                            anchors.fill: parent
                                            anchors.margins: 2
                                            Rectangle {
                                                width: parent.width * 1 / 8; height: parent.height;
                                                color: outRect._sideBackgroundColor
                                                radius: outRect._sideRadius
                                                CustomButton {
                                                    anchors.centerIn: parent
                                                    text: "-"
                                                    onClicked: {
                                                        slider.decrease()
                                                    }
                                                }
                                            }
                                            Rectangle {
                                                width: parent.width * 3 / 8; height: parent.height;
                                                color: outRect._sideBackgroundColor
                                                radius: outRect._sideRadius
                                                CustomText {
                                                    anchors.centerIn: parent
                                                    text: "干扰功率:"
                                                }
                                            }
                                            Rectangle {
                                                width: parent.width * 3 / 8; height: parent.height;
                                                color: outRect._sideBackgroundColor
                                                radius: outRect._sideRadius
                                                CustomText {
                                                    anchors.fill: parent
                                                    anchors.centerIn: parent
                                                    text: slider.value.toFixed(0)
                                                }
                                            }
                                            Rectangle {
                                                width: parent.width * 1 / 8; height: parent.height;
                                                color: outRect._sideBackgroundColor
                                                radius: outRect._sideRadius
                                                CustomButton {
                                                    anchors.centerIn: parent
                                                    text: "+"
                                                    onClicked: {
                                                        slider.increase()
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                width: parent.width; height: parent.height * 1 / 5.1
                                border.color: "black"
                                border.width: 2
                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 2
                                    spacing: 1
                                    Rectangle {
                                        width: parent.width / 5.1; height: parent.height;
                                        CustomComboBox {
                                           anchors.fill: parent
                                           anchors.centerIn: parent
                                           model: ["手动", "自动"]

                                           onActivated: {
                                               console.log("index ", model[index])
                                           }
                                        }
                                    }
                                    /* 此矩形当前是规避前面的CustomComboBox切换对下面Row组件字体消失的影响 */
                                    Rectangle {
                                        width: parent.width / 5.1 / 100; height: parent.height
                                    }
                                    Rectangle {
                                        width: parent.width / 5.1; height: parent.height
                                        CustomButton {
                                            anchors.fill: parent
                                            anchors.centerIn: parent
                                            text: "待机"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 5.1; height: parent.height
                                        CustomButton {
                                            anchors.fill: parent
                                            anchors.centerIn: parent
                                            text: "复位"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 5.1; height: parent.height
                                        CustomButton {
                                            anchors.fill: parent
                                            anchors.centerIn: parent
                                            text: "侦测"
                                        }
                                    }
                                    Rectangle {
                                        width: parent.width / 5.1; height: parent.height
                                        CustomButton {
                                            anchors.fill: parent
                                            anchors.centerIn: parent
                                            text: "干扰"
                                        }
                                    }
                                }
                            }
                        }
                    }

                    /* 处置 */
                    Rectangle {
                        border.color: "green"

                        Rectangle {
                            id:                 historyList
                            anchors.fill:       parent

                            ListView {
                                anchors.fill: parent
                                anchors.margins: 0

                                clip: true

                                model: 5

                                delegate: numberDelegate
                                spacing: 2

                                header: headerComponent
                                footer: footerComponent

                                ScrollBar.vertical: ScrollBar {}
                            }

                            Component {
                                id: headerComponent

                                Rectangle {
                                    width:                  historyList.width
                                    height:                 20
                                    color:                  "teal"

                                    Row {
                                        anchors.fill: parent
                                        Repeater {
                                            model: ["序号", "时标(s)", "方位", "频点", "带宽", "功率", "干扰"]
                                            Text {
                                                text: modelData + "  "
                                            }
                                        }
                                    }
                                }
                            }

                            Component {
                                id: footerComponent

                                Rectangle {
                                    width:                  historyList.width
                                    height:                 20
                                    color:                  "Purple"

                                    Text {
                                        anchors.centerIn:   parent
                                        font.pixelSize:     10
                                        text:               'footer'
                                    }
                                }
                            }

                            Component {
                                id: numberDelegate

                                CustomSwitchDelegate {
                                    width: historyList.width
                                    text: index + '    ' + Math.random().toFixed(2) + '    ' + Math.random().toFixed(2)
                                          + '    ' + Math.random().toFixed(2) + '    ' + Math.random().toFixed(2)
                                          + '    ' + Math.random().toFixed(2)
                                    onClicked: {
                                        console.log("--------------------start or stop----------------")
                                        console.log(JSON.stringify(modelData))
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

