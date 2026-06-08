import QtQuick

/*
    Built-in Fade hover animation template.

    Runtime implementation is currently wired directly in DockItem.qml so it
    can participate in the Mexican-wave neighbour effect.
*/
Item {
    id: root
    property bool hovered: false
    property real waveStrength: 0.0
    property int iconSize: 48
    width: iconSize + 26
    height: iconSize + 26
}
