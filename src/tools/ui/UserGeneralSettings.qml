import QtQuick                  2.3
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs          1.2
import QtQuick.Layouts          1.2

import QGroundControl                       1.0
import QGroundControl.FactSystem            1.0
import QGroundControl.FactControls          1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import QGroundControl.Controllers           1.0
import QGroundControl.SettingsManager       1.0

Rectangle {
    id:                 _root
    color:              qgcPal.window
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    property Fact _savePath:                            QGroundControl.settingsManager.appSettings.savePath
    property Fact _appFontPointSize:                    QGroundControl.settingsManager.appSettings.appFontPointSize
    property Fact _userBrandImageIndoor:                QGroundControl.settingsManager.brandImageSettings.userBrandImageIndoor
    property Fact _userBrandImageOutdoor:               QGroundControl.settingsManager.brandImageSettings.userBrandImageOutdoor
    property Fact _virtualJoystick:                     QGroundControl.settingsManager.appSettings.virtualJoystick
    property Fact _virtualJoystickAutoCenterThrottle:   QGroundControl.settingsManager.appSettings.virtualJoystickAutoCenterThrottle

    property real   _labelWidth:                ScreenTools.defaultFontPixelWidth * 20
    property real   _comboFieldWidth:           ScreenTools.defaultFontPixelWidth * 30
    property real   _valueFieldWidth:           ScreenTools.defaultFontPixelWidth * 10
    property string _mapProvider:               QGroundControl.settingsManager.flightMapSettings.mapProvider.value
    property string _mapType:                   QGroundControl.settingsManager.flightMapSettings.mapType.value
    property Fact   _followTarget:              QGroundControl.settingsManager.appSettings.followTarget
    property real   _panelWidth:                _root.width * _internalWidthRatio
    property real   _margins:                   ScreenTools.defaultFontPixelWidth
    property var    _planViewSettings:          QGroundControl.settingsManager.planViewSettings
    property var    _flyViewSettings:           QGroundControl.settingsManager.flyViewSettings
    property var    _videoSettings:             QGroundControl.settingsManager.videoSettings
    property string _videoSource:               _videoSettings.videoSource.rawValue
    property bool   _isGst:                     QGroundControl.videoManager.isGStreamer
    property bool   _isUDP264:                  _isGst && _videoSource === _videoSettings.udp264VideoSource
    property bool   _isUDP265:                  _isGst && _videoSource === _videoSettings.udp265VideoSource
    property bool   _isRTSP:                    _isGst && _videoSource === _videoSettings.rtspVideoSource
    property bool   _isTCP:                     _isGst && _videoSource === _videoSettings.tcpVideoSource
    property bool   _isMPEGTS:                  _isGst && _videoSource === _videoSettings.mpegtsVideoSource
    property bool   _videoAutoStreamConfig:     QGroundControl.videoManager.autoStreamConfigured
    property bool   _showSaveVideoSettings:     _isGst || _videoAutoStreamConfig
    property bool   _disableAllDataPersistence: QGroundControl.settingsManager.appSettings.disableAllPersistence.rawValue

    property string gpsDisabled: "Disabled"
    property string gpsUdpPort:  "UDP Port"

    readonly property real _internalWidthRatio: 0.8

        QGCFlickable {
            clip:               true
            anchors.fill:       parent
            contentHeight:      outerItem.height
            contentWidth:       outerItem.width

            Item {
                id:     outerItem
                width:  Math.max(_root.width, settingsColumn.width)
                height: settingsColumn.height

                ColumnLayout {
                    id:                         settingsColumn
                    anchors.horizontalCenter:   parent.horizontalCenter

                    QGCLabel {
                        id:         flyViewSectionLabel
                        text:       qsTr("Fly View")
                        visible:    QGroundControl.settingsManager.flyViewSettings.visible
                    }
                    Rectangle {
                        Layout.preferredHeight: flyViewCol.height + (_margins * 2)
                        Layout.preferredWidth:  flyViewCol.width + (_margins * 2)
                        color:                  qgcPal.windowShade
                        visible:                flyViewSectionLabel.visible
                        Layout.fillWidth:       true

                        ColumnLayout {
                            id:                         flyViewCol
                            anchors.margins:            _margins
                            anchors.top:                parent.top
                            anchors.horizontalCenter:   parent.horizontalCenter
                            spacing:                    _margins

                            GridLayout {
                                id:         videoGrid
                                columns:    2
                                visible:    _videoSettings.visible

                                QGCLabel {
                                    text:               qsTr("Video Settings")
                                    Layout.columnSpan:  2
                                    Layout.alignment:   Qt.AlignHCenter
                                }

                                QGCLabel {
                                    id:         videoSourceLabel
                                    text:       qsTr("Source")
                                    visible:    !_videoAutoStreamConfig && _videoSettings.videoSource.visible
                                }
                                FactComboBox {
                                    id:                     videoSource
                                    Layout.preferredWidth:  _comboFieldWidth
                                    indexModel:             false
                                    fact:                   _videoSettings.videoSource
                                    visible:                videoSourceLabel.visible
                                }

                                QGCLabel {
                                    id:         udpPortLabel
                                    text:       qsTr("UDP Port")
                                    visible:    !_videoAutoStreamConfig && (_isUDP264 || _isUDP265 || _isMPEGTS) && _videoSettings.udpPort.visible
                                }
                                FactTextField {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.udpPort
                                    visible:                udpPortLabel.visible
                                }

                                QGCLabel {
                                    id:         rtspUrlLabel
                                    text:       qsTr("RTSP URL")
                                    visible:    !_videoAutoStreamConfig && _isRTSP && _videoSettings.rtspUrl.visible
                                }
                                FactTextField {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.rtspUrl
                                    visible:                rtspUrlLabel.visible
                                }

                                QGCLabel {
                                    id:         tcpUrlLabel
                                    text:       qsTr("TCP URL")
                                    visible:    !_videoAutoStreamConfig && _isTCP && _videoSettings.tcpUrl.visible
                                }
                                FactTextField {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.tcpUrl
                                    visible:                tcpUrlLabel.visible
                                }

                                QGCLabel {
                                    text:                   qsTr("Aspect Ratio")
                                    visible:                !_videoAutoStreamConfig && _isGst && _videoSettings.aspectRatio.visible
                                }
                                FactTextField {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.aspectRatio
                                    visible:                !_videoAutoStreamConfig && _isGst && _videoSettings.aspectRatio.visible
                                }

                                QGCLabel {
                                    id:         videoFileFormatLabel
                                    text:       qsTr("Record File Format")
                                    visible:    _showSaveVideoSettings && _videoSettings.recordingFormat.visible
                                }
                                FactComboBox {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.recordingFormat
                                    visible:                videoFileFormatLabel.visible
                                }

                                QGCLabel {
                                    id:         maxSavedVideoStorageLabel
                                    text:       qsTr("Max Storage Usage")
                                    visible:    _showSaveVideoSettings && _videoSettings.maxVideoSize.visible && _videoSettings.enableStorageLimit.value
                                }
                                FactTextField {
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.maxVideoSize
                                    visible:                _showSaveVideoSettings && _videoSettings.enableStorageLimit.value && maxSavedVideoStorageLabel.visible
                                }

                                QGCLabel {
                                    id:         videoDecodeLabel
                                    text:       qsTr("Video decode priority")
                                    visible:    forceVideoDecoderComboBox.visible
                                }
                                FactComboBox {
                                    id:                     forceVideoDecoderComboBox
                                    Layout.preferredWidth:  _comboFieldWidth
                                    fact:                   _videoSettings.forceVideoDecoder
                                    visible:                fact.visible
                                    indexModel:             false
                                }

                                Item { width: 1; height: 1}
                                FactCheckBox {
                                    text:       qsTr("Disable When Disarmed")
                                    fact:       _videoSettings.disableWhenDisarmed
                                    visible:    !_videoAutoStreamConfig && _isGst && fact.visible
                                }

                                Item { width: 1; height: 1}
                                FactCheckBox {
                                    text:       qsTr("Low Latency Mode")
                                    fact:       _videoSettings.lowLatencyMode
                                    visible:    !_videoAutoStreamConfig && _isGst && fact.visible
                                }

                                Item { width: 1; height: 1}
                                FactCheckBox {
                                    text:       qsTr("Auto-Delete Saved Recordings")
                                    fact:       _videoSettings.enableStorageLimit
                                    visible:    _showSaveVideoSettings && fact.visible
                                }
                            }
                        }
                    }
                } // settingsColumn
            }
    }
}
