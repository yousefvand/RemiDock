import QtQuick

Item {
    id: root
    property string iconSource: ""
    property int iconSize: 48
    property bool hovered: false
    property bool verticalDock: false
    property string dockEdge: "bottom"
    property real visualOffset: 0
    property real visualSideOffset: 0
    property real visualRotation: 0
    property real visualScale: 1.0

    width: iconSize + 54
    height: iconSize + 54
    clip: false

    Timer {
        id: flicker
        interval: 70
        repeat: true
        running: root.hovered
        property real n: 0.3
        onTriggered: n = Math.random()
    }

    Rectangle {
        visible: root.hovered
        anchors.centerIn: parent
        width: root.iconSize + 28 + flicker.n * 12
        height: width
        radius: width / 2
        opacity: 0.24 + flicker.n * 0.22
        color: "#ff4a00"
    }

    Rectangle {
        visible: root.hovered
        anchors.centerIn: parent
        width: root.iconSize + 14 + flicker.n * 8
        height: width
        radius: width / 2
        opacity: 0.30
        color: "#ffd35a"
    }

    Text {
        visible: root.hovered
        text: "🔥"
        font.pixelSize: Math.max(20, root.iconSize * 0.42)
        opacity: 0.9
        anchors.horizontalCenter: parent.horizontalCenter
        y: root.dockEdge === "top" ? root.iconSize + 6 : -6 - flicker.n * 7
    }

    Image {
        anchors.centerIn: parent
        width: root.iconSize
        height: root.iconSize
        source: root.iconSource
        smooth: true
        mipmap: true
        rotation: root.visualRotation + (root.hovered ? Math.sin(flicker.n * 6.28) * 5 : 0)
        scale: root.visualScale * (root.hovered ? 1.18 : 1.0)
        transformOrigin: Item.Center

        Behavior on scale { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }

        transform: Translate {
            x: root.verticalDock
               ? (root.dockEdge === "left" ? -root.visualOffset : root.visualOffset)
               : root.visualSideOffset
            y: root.verticalDock
               ? root.visualSideOffset
               : (root.dockEdge === "top" ? -root.visualOffset : root.visualOffset)
        }
    }
}
