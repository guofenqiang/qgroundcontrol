import QtQuick 2.15

Item {
    id: fonts
    readonly property FontLoader fontAwesomeSolid: FontLoader {
        source: "qrc:/qml/CustomTools/Fonts/Font Awesome 6 Free-Solid-900.otf"
    }
    readonly property string solid: fontAwesomeSolid.name
}
