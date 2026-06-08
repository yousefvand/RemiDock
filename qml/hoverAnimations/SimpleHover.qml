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

    width: iconSize + 24
    height: iconSize + 24
    clip: false

    Image {
        anchors.centerIn: parent
        width: root.iconSize
        height: root.iconSize
        source: root.iconSource
        smooth: true
        mipmap: true
        rotation: root.visualRotation
        scale: root.visualScale * (root.hovered ? 1.08 : 1.0)
        transformOrigin: Item.Center

        Behavior on scale { NumberAnimation { duration: 110; easing.type: Easing.OutQuad } }

        transform: Translate {
            x: root.verticalDock
               ? (root.dockEdge === "left" ? -root.visualOffset : root.visualOffset) + (root.hovered ? (root.dockEdge === "left" ? 3 : -3) : 0)
               : root.visualSideOffset
            y: root.verticalDock
               ? root.visualSideOffset
               : (root.dockEdge === "top" ? -root.visualOffset : root.visualOffset) + (root.hovered ? (root.dockEdge === "top" ? 3 : -3) : 0)
        }
    }
}
