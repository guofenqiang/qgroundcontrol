import QtQuick 2.15
import QtQuick.Controls 2.5

Text {
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

    // 应用自定义样式
    anchors.centerIn: parent
    font.family: customButton.fontFamily
    font.pointSize: customButton.fontSize
    color: customButton.fontColor
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
}
