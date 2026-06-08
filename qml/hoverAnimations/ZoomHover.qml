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

    width: iconSize + 44
    height: iconSize + 44
    clip: false

    Rectangle {
        visible: root.hovered
        anchors.centerIn: parent
        width: root.iconSize + 20
        height: width
        radius: width / 2
        color: "#33ffffff"
        opacity: 0.7
        scale: root.hovered ? 1.0 : 0.7
        Behavior on scale { NumberAnimation { duration: 120; easing.type: Easing.OutBack } }
    }

    Image {
        anchors.centerIn: parent
        width: root.iconSize
        height: root.iconSize
        source: root.iconSource
        smooth: true
        mipmap: true
        rotation: root.visualRotation
        scale: root.visualScale * (root.hovered ? 1.42 : 1.0)
        transformOrigin: Item.Center

        Behavior on scale { NumberAnimation { duration: 140; easing.type: Easing.OutBack } }

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
