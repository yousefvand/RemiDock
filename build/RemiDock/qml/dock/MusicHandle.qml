import QtQuick
import QtQuick.Controls
import RemiDock

Rectangle {
    id: handle

    width: 28
    height: 28
    radius: 14

    readonly property bool enabledForDance: dockController.musicDanceEnabled
    readonly property bool musicPlaying: mediaMonitor.playing
    readonly property bool analyzerRunning: audioAnalyzer.running
    readonly property real liveLevel: audioAnalyzer.level

    color: enabledForDance
           ? (analyzerRunning ? Qt.rgba(0.25, 1.0, 0.55, 0.35 + liveLevel * 0.45) : "#554c8dff")
           : "#333333"
    border.color: enabledForDance ? "#bbffffff" : "#777777"
    border.width: 1

    scale: analyzerRunning ? 1.0 + audioAnalyzer.beat * 0.18 : 1.0

    Behavior on scale { NumberAnimation { duration: 70 } }

    Text {
        anchors.centerIn: parent
        text: "♫"
        color: enabledForDance ? "white" : "#aaaaaa"
        font.pixelSize: 17
        font.bold: true
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: dockController.toggleMusicDance()
    }

    ToolTip.visible: mouse.containsMouse
    ToolTip.delay: 400
    ToolTip.text: !enabledForDance
                  ? "Music dance is OFF"
                  : analyzerRunning
                    ? "Music dance is ON: reacting to system audio"
                    : musicPlaying
                      ? "Music dance is ON, but audio monitor is not available. Install/check parec."
                      : "Music dance is ON, waiting for music"
}
