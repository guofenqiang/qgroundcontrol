/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                  2.12
import QtQuick.Controls         2.4
import QtQuick.Dialogs          1.3
import QtQuick.Layouts          1.12

import QtLocation               5.3
import QtPositioning            5.3
import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.Controls      1.0
import QGroundControl.Airspace      1.0
import QGroundControl.Airmap        1.0
import QGroundControl.Controllers   1.0
import QGroundControl.Controls      1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.Palette       1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Vehicle       1.0
import MyObj                        1.0
import UDPClient                    1.0
import UDPServer                    1.0
import Telecontrol                  1.0

// This is the ui overlay layer for the widgets/tools for Fly View
Item {
    id: _root

    property var    parentToolInsets
    property var    totalToolInsets:        _totalToolInsets
    property var    mapControl

    property var    _activeVehicle:         QGroundControl.multiVehicleManager.activeVehicle
    property var    _planMasterController:  globals.planMasterControllerFlyView
    property var    _missionController:     _planMasterController.missionController
    property var    _geoFenceController:    _planMasterController.geoFenceController
    property var    _rallyPointController:  _planMasterController.rallyPointController
    property var    _guidedController:      globals.guidedControllerFlyView
    property real   _margins:               ScreenTools.defaultFontPixelWidth / 2
    property real   _toolsMargin:           ScreenTools.defaultFontPixelWidth * 0.75
    property rect   _centerViewport:        Qt.rect(0, 0, width, height)
    property real   _rightPanelWidth:       ScreenTools.defaultFontPixelWidth * 30

    readonly property real _defaultTextHeight:  ScreenTools.defaultFontPixelHeight
    readonly property real _defaultTextWidth:   ScreenTools.defaultFontPixelWidth
    readonly property real _Margin:             _defaultTextWidth / 2
    readonly property real _columNumber:        3

    QGCToolInsets {
        id:                     _totalToolInsets
        leftEdgeTopInset:       toolStrip.leftInset
        leftEdgeCenterInset:    toolStrip.leftInset
        leftEdgeBottomInset:    parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset:      parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset:   parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset:   parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset:       parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset:     parentToolInsets.topEdgeCenterInset
        topEdgeRightInset:      parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset:    parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset:  mapScale.centerInset
        bottomEdgeRightInset:   0
    }

    FlyViewMissionCompleteDialog {
        missionController:      _missionController
        geoFenceController:     _geoFenceController
        rallyPointController:   _rallyPointController
    }

    Row {
        id:                 multiVehiclePanelSelector
        anchors.margins:    _toolsMargin
        anchors.top:        parent.top
        anchors.right:      parent.right
        width:              _rightPanelWidth
        spacing:            ScreenTools.defaultFontPixelWidth
        visible:            QGroundControl.multiVehicleManager.vehicles.count > 1 && QGroundControl.corePlugin.options.flyView.showMultiVehicleList

        property bool showSingleVehiclePanel:  !visible || singleVehicleRadio.checked

        QGCMapPalette { id: mapPal; lightColors: true }

        QGCRadioButton {
            id:             singleVehicleRadio
            text:           qsTr("Single")
            checked:        true
            textColor:      mapPal.text
        }

        QGCRadioButton {
            text:           qsTr("Multi-Vehicle")
            textColor:      mapPal.text
        }
    }

    MultiVehicleList {
        anchors.margins:    _toolsMargin
        anchors.top:        multiVehiclePanelSelector.bottom
        anchors.right:      parent.right
        width:              _rightPanelWidth
        height:             parent.height - y - _toolsMargin
        visible:            !multiVehiclePanelSelector.showSingleVehiclePanel
    }


    GuidedActionConfirm {
        anchors.margins:            _toolsMargin
        anchors.top:                parent.top
        anchors.horizontalCenter:   parent.horizontalCenter
        z:                          QGroundControl.zOrderTopMost
        guidedController:           _guidedController
        guidedValueSlider:          _guidedValueSlider
    }

    FlyViewInstrumentPanel {
        id:                         instrumentPanel
        anchors.margins:            _toolsMargin
        anchors.top:                multiVehiclePanelSelector.visible ? multiVehiclePanelSelector.bottom : parent.top
        anchors.right:              parent.right
        width:                      _rightPanelWidth
        spacing:                    _toolsMargin
        visible:                    QGroundControl.corePlugin.options.flyView.showInstrumentPanel && multiVehiclePanelSelector.showSingleVehiclePanel
        availableHeight:            parent.height - y - _toolsMargin

        property real rightInset: visible ? parent.width - x : 0
    }

    PhotoVideoControl {
        id:                     photoVideoControl
        anchors.margins:        _toolsMargin
        anchors.right:          parent.right
        width:                  _rightPanelWidth
        state:                  _verticalCenter ? "verticalCenter" : "topAnchor"
        states: [
            State {
                name: "verticalCenter"
                AnchorChanges {
                    target:                 photoVideoControl
                    anchors.top:            undefined
                    anchors.verticalCenter: _root.verticalCenter
                }
            },
            State {
                name: "topAnchor"
                AnchorChanges {
                    target:                 photoVideoControl
                    anchors.verticalCenter: undefined
                    anchors.top:            instrumentPanel.bottom
                }
            }
        ]

        property bool _verticalCenter: !QGroundControl.settingsManager.flyViewSettings.alternateInstrumentPanel.rawValue
    }

    TelemetryValuesBar {
        id:                 telemetryPanel
        x:                  recalcXPosition()
        anchors.margins:    _toolsMargin

        // States for custom layout support
        states: [
            State {
                name: "bottom"
                when: telemetryPanel.bottomMode

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: undefined
                    anchors.bottom: parent.bottom
                    anchors.right: undefined
                    anchors.verticalCenter: undefined
                }

                PropertyChanges {
                    target: telemetryPanel
                    x: recalcXPosition()
                }
            },

            State {
                name: "right-video"
                when: !telemetryPanel.bottomMode && photoVideoControl.visible

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: photoVideoControl.bottom
                    anchors.bottom: undefined
                    anchors.right: parent.right
                    anchors.verticalCenter: undefined
                }
            },

            State {
                name: "right-novideo"
                when: !telemetryPanel.bottomMode && !photoVideoControl.visible

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        ]

        function recalcXPosition() {
            // First try centered
            var halfRootWidth   = _root.width / 2
            var halfPanelWidth  = telemetryPanel.width / 2
            var leftX           = (halfRootWidth - halfPanelWidth) - _toolsMargin
            var rightX          = (halfRootWidth + halfPanelWidth) + _toolsMargin
            if (leftX >= parentToolInsets.leftEdgeBottomInset || rightX <= parentToolInsets.rightEdgeBottomInset ) {
                // It will fit in the horizontalCenter
                return halfRootWidth - halfPanelWidth
            } else {
                // Anchor to left edge
                return parentToolInsets.leftEdgeBottomInset + _toolsMargin
            }
        }
    }

    // 雷达显示界面
    Rectangle {
        id:                 ecmPanel
        anchors.left:       parent.left
        anchors.top:        parent.verticalCenter
        width:              parent.width / 4
        anchors.bottom:     parent.bottom
        anchors.margins:    _toolsMargin
        color:              'blue'
        visible:            _activeVehicle ? true : false

        Rectangle {
            id:                 rect
            anchors.fill:       parent
            anchors.margins:    _toolsMargin

            Rectangle {
                id:                 rectTop
                anchors.left:       parent.left
                anchors.top:        parent.top
                anchors.right:      parent.right
                height:             parent.height * 0.1
                border.color:       'teal'
                border.width:       5

                RowLayout {
                    anchors.fill:       parent

                    Text {
                        text:                   _activeVehicle ? "time_usec: " + _activeVehicle.time_usec + " us": "time_usec: us"
                        horizontalAlignment:    Text.AlignHCenter
                        Layout.fillWidth:       true
                    }

                    Text {
                        text:                   _activeVehicle ? "frame_number: " + _activeVehicle.frame_number : "frame_number: "
                        horizontalAlignment:    Text.AlignHCenter
                        Layout.fillWidth:       true
                    }
                }
            }

            Rectangle {
                id:                 rectMiddle
                anchors.left:       parent.left
                anchors.top:        rectTop.bottom
                anchors.right:      parent.right
                height:             parent.height * 0.8
                border.color:       'Purple'
                border.width:       5

                ListView {
                    id: ecmList
                    anchors.fill: parent

                    model: [
                        {
                            pwd_target_number:      _activeVehicle.pdw1_target_number,
                            pdw_rate:               _activeVehicle.pdw1_rate,
                            pdw_azimuth:            _activeVehicle.pdw1_azimuth,
                        },

                        {
                            pwd_target_number:      _activeVehicle.pdw2_target_number,
                            pdw_rate:               _activeVehicle.pdw2_rate,
                            pdw_azimuth:            _activeVehicle.pdw2_azimuth,
                        },

                        {
                            pwd_target_number:      _activeVehicle.pdw3_target_number,
                            pdw_rate:               _activeVehicle.pdw3_rate,
                            pdw_azimuth:            _activeVehicle.pdw3_azimuth,
                        },

                        {
                            pwd_target_number:      _activeVehicle.pdw4_target_number,
                            pdw_rate:               _activeVehicle.pdw4_rate,
                            pdw_azimuth:            _activeVehicle.pdw4_azimuth,
                        },

                        {
                            pwd_target_number:      _activeVehicle.pdw5_target_number,
                            pdw_rate:               _activeVehicle.pdw5_rate,
                            pdw_azimuth:            _activeVehicle.pdw5_azimuth,
                        },
                    ]

                    delegate: SwitchDelegate {
                        id: control
                        width: ecmList.width
                        text: 'pwd_target_number: ' + modelData.pwd_target_number + '   pdw_freq: ' + modelData.pdw_rate +  "    pdw_azimuth: " + modelData.pdw_azimuth
                        highlighted: ListView.isCurrentItem

                        onClicked: {
                            console.log("--------------------start or stop----------------")
                            _activeVehicle.radarCmd(3)
                            console.log(JSON.stringify(modelData))
                            ecmList.currentIndex = index
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }
            }

            Rectangle {
                id:                 rectBottom
                anchors.left:       parent.left
                anchors.top:        rectMiddle.bottom
                anchors.right:      parent.right
                height:             parent.height * 0.1
                border.color:       'fuchsia'
                border.width:       5

                ColumnLayout {
                    anchors.fill:   parent

                    RowLayout {
                        Button {
                            id:                     start
                            Layout.fillWidth:       true
                            text:                   "Radar Scan Start"

                            onClicked: {
                                console.log("Radar Scan Start")
                            }
                        }

                        Button {
                            id:                     stop
                            Layout.fillWidth:       true
                            text:                   "Radar Scan Stop"

                            onClicked: {
                                console.log("Radar Scan Stop")
                            }
                        }
                    }
                }
            }
        }
    }

    // 雷达控制界面
    Rectangle {
        id:                 ecmHistoryPanel
        anchors.right:      parent.right
        anchors.top:        parent.verticalCenter
        width:              parent.width * 0.25
        anchors.bottom:     parent.bottom
        anchors.margins:    _toolsMargin
        color:              'blue'
        visible:            _activeVehicle ? true : false

        Rectangle {
            anchors.fill:       parent
            anchors.margins:    _toolsMargin

            SwipeView {
                id:             view
                anchors.fill:   parent
                currentIndex:   0
                clip:           true

                //参数设置页面
                Item {
                    id:         firstPage

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

                        readonly property real columNumber: 3

                        QGCPalette { id: qgcPal }

                        Rectangle {
                            anchors.fill:           parent
                            color:                  parent.color
                            visible:                _activeVehicle ? true: false

                            Rectangle {
                                id:                 _rectTop
                                anchors.left:       parent.left
                                anchors.top:        parent.top
                                anchors.right:      parent.right
                                height:             parent.height * 0.2
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
                                id:                 _rectCenter1
                                anchors.left:       parent.left
                                anchors.top:        _rectTop.bottom
                                anchors.right:      parent.right
                                height:             parent.height * 0.6
                                color:              qgcPal.window

                                Rectangle {
                                    anchors.fill:           parent
                                    color:                  parent.color

                                    Rectangle {
                                        id:                 showFreqLeft
                                        anchors.left:       parent.left
                                        anchors.top:        parent.top
                                        anchors.bottom:     parent.bottom
                                        width:              parent.width * 0.7
                                        color:              qgcPal.window

                                        Rectangle {
                                            anchors.fill:           parent
                                            border.color:           'purple'
                                            border.width:           _Margin

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
                                        width:              parent.width * 0.3
                                        color:              qgcPal.window
                                        border.color:       'purple'
                                        border.width:       _Margin

                                        signal addFreq()
                                        signal clearFreq()

                                        ColumnLayout {
                                            anchors.fill:               parent
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

                            Rectangle {
                                id:                 _rectBottom
                                anchors.left:       parent.left
                                anchors.top:        _rectCenter1.bottom
                                anchors.right:      parent.right
                                height:             parent.height * 0.2
                                color:              qgcPal.window
                                border.color:       'yellow'
                                border.width:       _Margin

                                RowLayout {
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

                }

                //历史页面
                Item {
                    Rectangle {
                        id:                 ecmHistory
                        anchors.fill:       parent

                        Rectangle {
                            id:                 historyTop
                            anchors.left:       parent.left
                            anchors.top:        parent.top
                            anchors.right:      parent.right
                            height:             parent.height * 0.1
                            color:              'grey'

                            Rectangle {
                                anchors.fill:       parent
                                color:              parent.color

                                Text {
                                    anchors.centerIn:   parent
                                    font.pixelSize:     20
                                    text:               '历史记录'
                                    font.bold:          true
                                    font.family:        "微软雅黑"
                                }
                            }
                        }

                        Rectangle {
                            id:                 historyList
                            anchors.left:       parent.left
                            anchors.top:        historyTop.bottom
                            anchors.right:      parent.right
                            height:             parent.height * 0.8
                            color:              "white"

                            ListView {
                                anchors.fill: parent
                                anchors.margins: 0

                                clip: true

                                model: 20

                                delegate: numberDelegate
                                spacing: 5

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

                                    Text {
                                        anchors.centerIn:   parent
                                        font.pixelSize:     10
                                        text:               'header'
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

                                Rectangle {
                                    width:                  historyList.width
                                    height:                 40
                                    color:                  "lightGreen"

                                    Text {
                                        anchors.centerIn:   parent
                                        font.pixelSize:     10
                                        text:               index
                                    }
                                }
                            }
                        }

                        Rectangle {
                            id:                 historyBottom
                            anchors.left:       parent.left
                            anchors.top:        historyList.bottom
                            anchors.right:      parent.right
                            height:             parent.height * 0.1
                            color:              'grey'

                            Rectangle {
                                anchors.fill:       parent
                                color:              parent.color

                                Text {
                                    anchors.centerIn:   parent
                                    font.pixelSize:     20
                                    text:               '总共数据'
                                    font.bold:          true
                                    font.family:        "微软雅黑"
                                }
                            }
                        }
                    }
                }

                // 九院协议转换
                Item {
                    Rectangle {
                        anchors.fill: parent
                        color: 'red'

                        ColumnLayout {
                            spacing:            2
                            anchors.fill:       parent

                            Rectangle {
                                Layout.alignment: Qt.AlignCenter
                                Layout.preferredWidth:          parent.width
                                Layout.preferredHeight:         parent.height * 0.5
                                color:                          "green"

                                RowLayout {
                                    spacing:    2

                                    Button {
                                        text: "Ok"
                                        onClicked: {
                                            console.log("send data")
                                        }
                                    }

                                    Button {
                                        text: "Cancel"
                                        onClicked: {
                                            console.log("cancel")
                                        }
                                    }
                                }

                            }

                            Rectangle{
                               Layout.alignment: Qt.AlignCenter
                               Layout.preferredWidth:          parent.width
                               Layout.preferredHeight:         parent.height * 0.5
                               color:                          "yellow"

                            }

                        }

                        //会初始化 UDP类
                        UDPClient {
                            id: udpclient
                            objectName: "udpclient"
                            Component.onCompleted: {
                                console.log("init udpclient")
                            }
                        }

                        UDPServer {
                            id: udpserver
                            Component.onCompleted: {
                                console.log("init udpserver")
                            }
                        }

                    }
                }

                //test页面
                Item {
                    id: testPage

                    Rectangle{
                        anchors.fill: parent
                        color: "lightyellow"
                        Text{
                            anchors.centerIn: parent
                            text: "test页面"
                            font.pixelSize: 20
                        }
                    }

                    Rectangle {
                        id:                 testPanel
                        color:              'antiquewhite'
                        visible:            true

                        property int value: myobj.iValue

                        signal qmlSig(int i, string s)

                        function qmlSlot(i, s) {
                            console.log("qml", i, s);
                        }

                        Button {
                            objectName: "mybutton"
                            onClicked: {
                                myobj.cppSig(99, "lisi")
                                myobj.func()
                                testPanel.qmlSig(10, "zhangsan")
                                myobj._radarCmd(0)
                            }
                        }

                        MyObject {
                            id: myobj
                            iValue: 10
                            sString: "zhangsan"
                            Component.onCompleted: {
                                console.log(iValue, sString)
                            }
                        }

                        Connections {
                            target: myobj

                            function onCppSig(i, s) {
                                testPanel.qmlSlot(i, s)
                            }
                        }

                        Component.onCompleted: {
                            testPanel.qmlSig.connect(myobj.cppSlot)
                        }

                        onValueChanged: {
                            console.log(value)
                        }

                        Button {
                            x: 200
                            onClicked: {
                                console.log("------")
                                myobj.iValue = 20
                            }
                        }
                    }
                }
            }

            //页面底部指示器
            PageIndicator {
                id: indicator

                count: view.count
                currentIndex: view.currentIndex

                anchors.bottom: view.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

    // 仅为了在qml初始化Telecontrol类
//    Telecontrol {
//        id: telecontrol
//        objectName: "telecontrol"
//        Component.onCompleted: {
//            console.log("init telecontrol")
//        }
//    }

    //-- Virtual Joystick
    Loader {
        id:                         virtualJoystickMultiTouch
        z:                          QGroundControl.zOrderTopMost + 1
        width:                      parent.width  - (_pipOverlay.width / 2)
        height:                     Math.min(parent.height * 0.25, ScreenTools.defaultFontPixelWidth * 16)
        visible:                    _virtualJoystickEnabled && !QGroundControl.videoManager.fullScreen && !(_activeVehicle ? _activeVehicle.usingHighLatencyLink : false)
        anchors.bottom:             parent.bottom
        anchors.bottomMargin:       parentToolInsets.leftEdgeBottomInset + ScreenTools.defaultFontPixelHeight * 2
        anchors.horizontalCenter:   parent.horizontalCenter
        source:                     "qrc:/qml/VirtualJoystick.qml"
        active:                     _virtualJoystickEnabled && !(_activeVehicle ? _activeVehicle.usingHighLatencyLink : false)

        property bool autoCenterThrottle: QGroundControl.settingsManager.appSettings.virtualJoystickAutoCenterThrottle.rawValue

        property bool _virtualJoystickEnabled: QGroundControl.settingsManager.appSettings.virtualJoystick.rawValue
    }

    FlyViewToolStrip {
        id:                     toolStrip
        anchors.leftMargin:     _toolsMargin + parentToolInsets.leftEdgeCenterInset
        anchors.topMargin:      _toolsMargin + parentToolInsets.topEdgeLeftInset
        anchors.left:           parent.left
        anchors.top:            parent.top
        z:                      QGroundControl.zOrderWidgets
        maxHeight:              parent.height - y - parentToolInsets.bottomEdgeLeftInset - _toolsMargin
        visible:                !QGroundControl.videoManager.fullScreen

        onDisplayPreFlightChecklist: preFlightChecklistPopup.createObject(mainWindow).open()

        property real leftInset: x + width
    }

    Rectangle{
        //该控件的左侧和 toolStrip 的左侧对齐
        anchors.left: toolStrip.left
        //该控件的顶部位于 toolStrip 的底部
        anchors.top: toolStrip.bottom
        //控件顶部设置一个留白间隔
        anchors.topMargin: _margins * 2
        //设置控件的宽度、高度、颜色
        width: 200
        height: 40
        color: "red"
        //为控件设置一个圆弧
        radius: 4
        visible: false

        //为控件设置层级，层级低的可能会被层级高的遮挡而无法看到
//        z: _panel.z + 4

        Text{
            //为控件添加一个 text 区域，进行文本显示
            anchors.fill: parent
            text: qsTr("Request All Parameters")
        }

        MouseArea{
            //为整个控件覆盖一个鼠标操作区域，鼠标点击触发 clicked 事件，通过 onClicked 处理相关的事件
            anchors.fill: parent

            onClicked: {

//                console.log("Request all Parameter is clicked!")
//                _activeVehicle.requestAllParameters()

                _activeVehicle.selfCheck()
            }
        }
    }

    FlyViewAirspaceIndicator {
        anchors.top:                parent.top
        anchors.topMargin:          ScreenTools.defaultFontPixelHeight * 0.25
        anchors.horizontalCenter:   parent.horizontalCenter
        z:                          QGroundControl.zOrderWidgets
        show:                       mapControl.pipState.state !== mapControl.pipState.pipState
    }

    VehicleWarnings {
        anchors.centerIn:   parent
        z:                  QGroundControl.zOrderTopMost
    }

    MapScale {
        id:                 mapScale
        anchors.margins:    _toolsMargin
        anchors.left:       toolStrip.right
        anchors.top:        parent.top
        mapControl:         _mapControl
        buttonsOnLeft:      false
        visible:            !ScreenTools.isTinyScreen && QGroundControl.corePlugin.options.flyView.showMapScale && mapControl.pipState.state === mapControl.pipState.fullState

        property real centerInset: visible ? parent.height - y : 0
    }

    Component {
        id: preFlightChecklistPopup
        FlyViewPreFlightChecklistPopup {
        }
    }
}
