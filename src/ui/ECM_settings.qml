import QtQuick 2.15
import QtQuick.Controls 1.2
import QtQuick.Window 2.15

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QtQuick.Layouts 1.15

Rectangle {
    id: window
    width: parent.width
    height: parent.height
    visible: true

    Item {
        anchors.fill: parent

        Rectangle {
            id: rectTop
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: parent.height / 3
            color: 'red'

            Item {
                id: root
                anchors.fill: parent

                ComboBox {
                    model: ["自动", "手动"]

                    onActivated: {
                        console.log("index ", model[index])
                    }
                }
            }
        }

        Rectangle {
            id: rectCenter
            anchors.left: parent.left
            anchors.top: rectTop.bottom
            anchors.right: parent.right
            height: parent.height / 3
            color: 'green'

            RowLayout {
                TextField {
                    placeholderText: qsTr("Enter frequency min")
                }

                TextField {
                    placeholderText: qsTr("Enter frequency max")
                }
            }
        }

        Rectangle {
            id: rectBottom
            anchors.left: parent.left
            anchors.top: rectCenter.bottom
            anchors.right: parent.right
            height: parent.height / 3
            color: 'blue'

            TextField {
                placeholderText: qsTr("Enter power")
            }

        }
    }
}
