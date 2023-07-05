import QtQuick 2.15
import QtQuick.Controls 1.2
import QtQuick.Window 2.15

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QtQuick.Layouts 1.15
import QGroundControl.Vehicle       1.0

Rectangle {
    id: window
    anchors.fill:       parent
    visible: true
    color:  qgcPal.window
    z:      QGroundControl.zOrderTopMost

    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle

    readonly property real _defaultTextHeight:  ScreenTools.defaultFontPixelHeight
    readonly property real _defaultTextWidth:   ScreenTools.defaultFontPixelWidth
    readonly property real _Margin:             _defaultTextWidth / 2
    readonly property real _horizontalMargin:   _defaultTextWidth / 2
    readonly property real _verticalMargin:     _defaultTextHeight / 2
    readonly property real _buttonHeight:       ScreenTools.isTinyScreen ? ScreenTools.defaultFontPixelHeight * 3 : ScreenTools.defaultFontPixelHeight * 2

    property bool _first: true

    property bool _commingFromRIDSettings:  false

    readonly property real columNumber: 3

    QGCPalette { id: qgcPal }

    Rectangle {
        anchors.fill:           parent
        color:                  parent.color
        visible:                _activeVehicle ? true: false

        Rectangle {
            id:                 rectTop
            anchors.left:       parent.left
            anchors.top:        parent.top
            anchors.right:      parent.right
            height:             parent.height / columNumber
            color:              qgcPal.window
            border.color:       'yellow'
            border.width:       _Margin

            ComboBox {
                anchors.horizontalCenter:   parent.horizontalCenter
                anchors.verticalCenter:     parent.verticalCenter
                model: ["自动", "手动"]

                onActivated: {
                    console.log("index ", model[index])
                }
            }
        }

        Rectangle {
            id:                 rectCenter1
            anchors.left:       parent.left
            anchors.top:        rectTop.bottom
            anchors.right:      parent.right
            height:             parent.height / columNumber
            color:              qgcPal.window
            border.color:       'yellow'
            border.width:       _Margin

            ColumnLayout {
                anchors.horizontalCenter:   parent.horizontalCenter
                anchors.verticalCenter:     parent.verticalCenter

                TextField {
                    id:     freq_min
                    placeholderText: qsTr("Enter frequency min")
                    validator: IntValidator{bottom: 800; top: 6000;}

                    onAccepted: {
                        console.log("min freq changed to:", text)
                    }
                }

                TextField {
                    id:     freq_max
                    placeholderText: qsTr("Enter frequency max")
                    validator: IntValidator{bottom: 800; top: 6000;}

                    onAccepted: {
                        console.log("max freq changed to:", text)
                    }
                }

                Button {
                    text:               "add"

                    onClicked: {
                        console.log("--------------------set frequency----------------")
                        _activeVehicle.radarCmd(2)
                        console.log("freq min:", freq_min.text, "   freq max:", freq_max.text)
                    }
                }
            }
        }

//        Rectangle {
//            id:                 rectCenter2
//            anchors.left:       parent.left
//            anchors.top:        rectCenter1.bottom
//            anchors.right:      parent.right
//            height:             parent.height / columNumber
//            color:              qgcPal.window
//            border.color:       'yellow'
//            border.width:       _Margin

//            TextField {
//                anchors.horizontalCenter:   parent.horizontalCenter
//                anchors.verticalCenter:     parent.verticalCenter

//                placeholderText: qsTr("Enter power")

//                onAccepted: {
//                    console.log("power changed to:", text)
//                }
//            }
//        }

        Rectangle {
            id:                 rectBottom
            anchors.left:       parent.left
            anchors.top:        rectCenter1.bottom
            anchors.right:      parent.right
            height:             parent.height / columNumber
            color:              qgcPal.window
            border.color:       'yellow'
            border.width:       _Margin

            ColumnLayout {
                anchors.horizontalCenter:   parent.horizontalCenter
                anchors.verticalCenter:     parent.verticalCenter

                Button {
                    text:               "standby"

                    onClicked: {
                        console.log("--------------------standby----------------")
                        _activeVehicle.radarCmd(0)
                    }
                }

                Button {
                    text:               "reset"

                    onClicked: {
                        console.log("--------------------reset----------------")
                        _activeVehicle.radarCmd(1)
                    }
                }
            }
        }
    }
}
