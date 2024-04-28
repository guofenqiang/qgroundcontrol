import QtQuick 2.3

import UserTools.ui 1.0

Item {
    anchors.fill: parent
    Column {
        anchors.fill: parent
        spacing: 2
        Rectangle {
            width: parent.width
            height: parent.height / 2

            UserLinkSettings {

            }
        }

        Rectangle {
            width: parent.width
            height: parent.height / 2

            UserGeneralSettings {

            }
        }
    }
}
