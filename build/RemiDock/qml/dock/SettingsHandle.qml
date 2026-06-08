import QtQuick
import QtQuick.Controls

Rectangle {
    id: handle

    width: 28
    height: 28
    radius: 14

    color: mouse.containsMouse ? "#55ffffff" : "#22ffffff"
    border.color: "#88ffffff"
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: "⋮"
        color: "white"
        font.pixelSize: 22
        font.bold: true
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: dockController.toggleSettings()
    }

    ToolTip.visible: mouse.containsMouse
    ToolTip.delay: 400
    ToolTip.text: "Settings"
}
