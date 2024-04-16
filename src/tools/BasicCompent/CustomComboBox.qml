import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4

import CustomTools.Fonts 1.0

ComboBox {
    id: customComboBox

    // 默认背景颜色
    property color backgroundColor: "#2B3042"

    // 默认边框颜色
    property string borderColor: "#00FFFF"

    // 默认字体颜色
    property color fontColor: "#00FFFF"

    // 默认字体
    property string fontFamily: "Arial"
    property int fontSize: 15

    // 默认弧度
    property int radius: 10

    // 自定义下拉指示器颜色属性
    property color dropDownIndicatorColor: "#007BFF"

    Fonts {
        id: fonts
    }

    background: Rectangle {
        implicitWidth: customComboBox.width
        implicitHeight: customComboBox.height
        color: customComboBox.backgroundColor
        border.color: customComboBox.borderColor
        radius: customComboBox.radius
    }

    indicator: Text {
        text: "\uf0d7"
        font.family: fonts.solid
        font.pixelSize: 25
        anchors {
            top: parent.top
            topMargin: (customComboBox.availableHeight - font.pixelSize) / 2
            bottom: parent.bottom
            right: parent.right
            rightMargin: 5
        }

        color: customComboBox.pressed ? customComboBox.fontColor : Qt.darker(customComboBox.fontColor, 1.1)
        rotation: down ? 180 : 0
        Behavior on rotation {
            NumberAnimation { duration: 200 }
        }
    }

    contentItem: Text {
        text: displayText
        color: customComboBox.fontColor
        font.family: customComboBox.fontFamily
        font.pointSize: customComboBox.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
