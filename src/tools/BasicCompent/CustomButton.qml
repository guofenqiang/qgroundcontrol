import QtQuick 2.15
import QtQuick.Controls 2.15

import CustomTools.Fonts 1.0

Button {
    id: customButton
    anchors.fill: parent

    // 默认背景颜色
    property color backgroundColor: "#2B3042"

    // 默认边框颜色
    property string borderColor: "#00FFFF"

    // 默认字体颜色
    property color fontColor: "#00FFFF"

    // 默认字体
    property string fontFamily: "Helvetica"
    property int fontSize: 16

    // 默认弧度
    property int radius: 10

    Fonts {
        id: fonts
    }

    // 应用自定义样式
    background: Rectangle {
        color: customButton.backgroundColor
        border.color: customButton.borderColor
        radius: customButton.radius
        gradient: Gradient {
            GradientStop {color: customButton.pressed ? backgroundColor : Qt.darker(backgroundColor, 1.1) }
        }
    }

    contentItem: Text {
        text: customButton.text
        font.family: customButton.fontFamily
        font.pointSize: customButton.fontSize
        color: customButton.fontColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    indicator: Text {
        text: "\uf8cc"
        font.family: fonts.solid
        font.pixelSize: 10
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 2
        }

        color: customButton.pressed ? customButton.fontColor : Qt.darker(customButton.fontColor, 1.1)
    }
}
