import QtQuick 2.15
//import QtQuick.Controls 1.2
import QtQuick.Controls 2.15
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
                model: ["手动", "自动"]

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

            Rectangle {
                anchors.fill:           parent
                color:                  parent.color

                Rectangle {
                    id:                 showFreqLeft
                    anchors.left:       parent.left
                    anchors.top:        parent.top
                    anchors.bottom:     parent.bottom
                    width:              parent.width * 0.5
                    color:              qgcPal.window

                    Rectangle {
                        anchors.fill:           parent
                        anchors.leftMargin:     _Margin * 40
                        anchors.rightMargin:    _Margin * 40
                        anchors.topMargin:      _Margin * 2
                        anchors.bottomMargin:   _Margin * 2

                        Component {
                            id: headerView
                            Item {
                                width: parent.width
                                height: 30
                                RowLayout {
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 8

                                    Text {
                                        text: "index" + "\t" + "freq min" + "\t" + "freq max"
                                        font.bold: true
                                        font.pixelSize: 20
                                        Layout.preferredWidth: 120
                                    }
                                }
                            }
                        }

                        Component {
                            id: footerView
                            Item{
                                id: footerRootItem
                                width: parent.width
                                height: 30
                                property alias text: txt.text

                                // 1.自定义信号
                                signal clean()
                                signal add()

                                Text {
                                    anchors.left: parent.left
                                    anchors.top: parent.top
                                    anchors.bottom: parent.bottom
                                    id: txt
                                    font.italic: true
                                    color: "blue"
                                    verticalAlignment: Text.AlignVCenter
                                }

                                Button {
                                    id: clearAll
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: "Clear all"
                                    onClicked: footerRootItem.clean()
                                }

//                                Button {
//                                    id: addOne
//                                    anchors.right: clearAll.left
//                                    anchors.rightMargin: 4
//                                    anchors.verticalCenter: parent.verticalCenter
//                                    text: "Add"
//                                    onClicked: footerRootItem.add()
//                                }
                            }
                        }

                        Component {
                            id: phoneDelegate
                            Item {
                                id: wrapper
                                width: 360
                                height: 30

                                MouseArea {
                                    anchors.fill: parent

                                    onClicked: {
                                        wrapper.ListView.view.currentIndex = index
                                        mouse.accepted = true
                                    }

                                    onDoubleClicked: {
                                        wrapper.ListView.view.model.remove(index)
                                        mouse.accepted = true
                                        _activeVehicle.radarCmd(2)
                                    }
                                }

                                RowLayout {
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 8

                                    Text {
                                        id: col1
                                        text: name + "\t" + cost + "\t" +  manufacturer
                                        color: wrapper.ListView.isCurrentItem ? "green" : "black"
                                        font.pixelSize: wrapper.ListView.isCurrentItem ? 22 : 18
                                        Layout.preferredWidth: 120
                                    }
                                }
                            }
                        }

                        Component {
                            id: phoneModel;
                            ListModel {
                                ListElement{
                                    name: "频段"
                                    cost: "800"
                                    manufacturer: "6000"
                                }
                                // 省略。。。
                            }
                        }

                        ListView {
                            id: listView
                            anchors.fill: parent

                            delegate: phoneDelegate
                            model: phoneModel.createObject(listView)
                            header: headerView
                            footer: footerView
                            focus: true
                            highlight: Rectangle{
                                color: "lightblue"
                            }

                            onCurrentIndexChanged: {
                                if( listView.currentIndex >=0 ){
                                    var data = listView.model.get(listView.currentIndex)
                                    listView.footerItem.text = data.name + " , " + data.cost + " , " + data.manufacturer
                                }else{
                                    listView.footerItem.text = ""
                                }
                            }

                            // 2.槽函数：添加数据
                            function addOne() {
                                model.append(
                                            {
                                                "name": "频段",
                                                "cost": freq_min.text,
                                                "manufacturer": freq_max.text
                                            }
                                )
                            }

                            function cleanAll() {
                                footerRootItem.clean()
                            }

                            // 3.连接信号槽
                            Component.onCompleted: {
                                listView.footerItem.clean.connect(listView.model.clear)
                                listView.footerItem.add.connect(listView.addOne)
                            }
                        }
                    }
                }

                Rectangle {
                    id:                 showAddRight
                    anchors.left:       showFreqLeft.right
                    anchors.top:        parent.top
                    anchors.bottom:     parent.bottom
                    width:              parent.width * 0.5
                    color:              qgcPal.window

                    signal addFreq()
                    signal clearFreq()

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
                            text:               "Add"

                            onClicked: {
                                console.log("--------------------set frequency----------------")
                                _activeVehicle.radarCmd(2)
                                showAddRight.addFreq()
                                console.log("freq min:", freq_min.text, "   freq max:", freq_max.text)
                            }
                        }

//                        Button {
//                            text:               "Clear all"

//                            onClicked: {
//                                console.log("--------------------set frequency----------------")
//                                _activeVehicle.radarCmd(2)
//                                showAddRight.clearFreq()
//                                console.log("--------------------clear all frequency band----------------")
//                            }
//                        }

                        Component.onCompleted: {
                            showAddRight.addFreq.connect(listView.addOne)
                            showAddRight.clearFreq.connect(listView.cleanAll)
                        }
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
