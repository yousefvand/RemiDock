import QtQuick
import QtQuick.Controls
import RemiDock

Rectangle {
    id: handle

    width: 28
    height: 28
    radius: 14

    color: area.containsMouse ? "#55ffffff" : "#22ffffff"
    border.color: "#88ffffff"
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: "✣"
        color: "white"
        font.pixelSize: 17
        font.bold: true
    }

    MouseArea {
        id: area

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            dockController.cycleEdge()
        }
    }

    ToolTip.visible: area.containsMouse
    ToolTip.delay: 400
    ToolTip.text: "Click to cycle dock edge"
}
