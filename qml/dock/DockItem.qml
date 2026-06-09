import QtQuick
import QtQuick.Controls
import RemiDock

Item {
    id: item

    property string appName: ""
    property string appIcon: "application-x-executable"
    property string appDesktopFile: ""
    property bool verticalDock: false
    property string dockEdge: "bottom"
    property bool dancing: false
    property int danceIndex: 0
    property real audioLevel: 0
    property real audioBeat: 0
    property real audioFast: 0
    property real audioSlow: 0
    property real audioPulse: 0
    property bool running: false
    property bool active: false
    property real waveStrength: 0.0

    signal launchRequested()
    signal removeRequested()
    signal hoverChanged(bool isHovered, int itemIndex)

    width: dockController.iconSize + 26
    height: dockController.iconSize + 26
    clip: false

    readonly property bool hovered: iconHoverHandler.hovered
    readonly property bool zoomMode: dockController.hoverAnimation === "zoom"
    readonly property bool fireMode: dockController.hoverAnimation === "fire"
    readonly property bool fadeMode: dockController.hoverAnimation === "fade"
    readonly property bool simpleMode: dockController.hoverAnimation === "simple" || dockController.hoverAnimation === ""

    readonly property real personality: 0.20 + ((danceIndex * 37) % 100) / 100.0
    readonly property real phase: ((danceIndex * 29) % 360) * Math.PI / 180.0
    readonly property real wobble: Math.sin(danceClock.t * (5.0 + personality * 7.0) + phase)
    readonly property real jitter: Math.sin(danceClock.t * (12.0 + personality * 15.0) + phase * 0.7)
    readonly property real localLevel: Math.max(0, Math.min(1, audioLevel * (0.45 + personality * 1.35) + audioFast * (0.10 + personality * 0.35)))
    readonly property real jump: dancing ? ((audioBeat > 0.22 && danceIndex % 4 !== 0) ? audioBeat : audioPulse * 0.35) : 0

    property real visualOffset: 0
    property real visualSideOffset: 0
    property real visualRotation: 0
    property real visualScale: 1.0

    property real hoverLift: waveStrength > 0 ? (simpleMode ? 5 : zoomMode ? 7 : fireMode ? 8 : fadeMode ? 6 : 5) * waveStrength : 0
    property real hoverScale: 1.0 + ((simpleMode ? 0.10 : zoomMode ? 0.62 : fireMode ? 0.20 : fadeMode ? 0.16 : 0.10) * waveStrength)
    property real hoverRotation: waveStrength > 0 && fireMode ? Math.sin(fireClock.t * 10.0) * 2.5 * waveStrength : 0
    property real hoverOpacity: fadeMode && waveStrength > 0 && waveStrength < 0.75 ? 0.25 : 1.0

    Behavior on hoverScale { NumberAnimation { duration: 130; easing.type: Easing.OutBack } }
    Behavior on hoverLift { NumberAnimation { duration: 120; easing.type: Easing.OutQuad } }

    Timer {
        id: danceClock
        interval: 33
        repeat: true
        running: item.dancing
        property real t: 0

        onTriggered: {
            t += interval / 1000.0
            var amp = Math.min(1.0, item.localLevel)
            var beatKick = item.jump
            item.visualOffset = -(amp * (2.5 + item.personality * 12.0) + beatKick * (5.0 + item.personality * 13.0))
            item.visualSideOffset = item.wobble * amp * (1.0 + item.personality * 5.0)
            item.visualRotation = item.jitter * amp * (2.0 + item.personality * 10.0)
            item.visualScale = 1.0 + beatKick * (0.02 + item.personality * 0.10)
        }
    }

    Timer {
        id: fireClock
        interval: 33
        repeat: true
        running: item.hovered && item.fireMode
        property real t: 0
        onTriggered: t += interval / 1000.0
    }

    onDancingChanged: {
        if (!dancing) {
            visualOffset = 0
            visualSideOffset = 0
            visualRotation = 0
            visualScale = 1.0
        }
    }

    function yDanceOffset() {
        var base = item.verticalDock ? item.visualSideOffset : (item.dockEdge === "top" ? -item.visualOffset : item.visualOffset)
        if (item.hovered) {
            if (!item.verticalDock)
                base += item.dockEdge === "top" ? item.hoverLift : -item.hoverLift
        }
        return base
    }

    function xDanceOffset() {
        var base = item.verticalDock ? (item.dockEdge === "left" ? -item.visualOffset : item.visualOffset) : item.visualSideOffset
        if (item.hovered) {
            if (item.verticalDock)
                base += item.dockEdge === "left" ? item.hoverLift : -item.hoverLift
        }
        return base
    }

    // Fire layer: sprite-sheet based animation.
    // This avoids relying on Qt AnimatedImage decoding APNG/WebP/GIF.
    Item {
        id: fireLayer
        z: 60
        visible: item.hovered && item.fireMode

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -dockController.iconSize * 0.12

        width: dockController.iconSize * 2.25
        height: dockController.iconSize * 2.25
        clip: false
        opacity: visible ? 0.96 : 0.0
        scale: visible ? 1.04 : 0.92

        property int frameCount: 16
        property int frameIndex: 0
        property int frameWidth: 150
        property int frameHeight: 150

        Timer {
            interval: 70
            repeat: true
            running: fireLayer.visible
            onTriggered: fireLayer.frameIndex = (fireLayer.frameIndex + 1) % fireLayer.frameCount
        }

        Image {
            anchors.fill: parent
            source: "qrc:/qt/qml/RemiDock/assets/animations/fire_spritesheet.png"
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
            sourceClipRect: Qt.rect(
                fireLayer.frameIndex * fireLayer.frameWidth,
                0,
                fireLayer.frameWidth,
                fireLayer.frameHeight
            )
        }

        onVisibleChanged: {
            if (visible)
                frameIndex = 0
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 90
                easing.type: Easing.OutQuad
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 110
                easing.type: Easing.OutBack
            }
        }
    }

    Image {
        id: iconImage
        z: 10
        anchors.centerIn: parent
        width: dockController.iconSize
        height: dockController.iconSize
        source: "image://icon/" + item.appIcon
        smooth: true
        mipmap: true
        transformOrigin: Item.Center
        opacity: (item.hovered && item.fireMode ? 0.82 : 1.0) * item.hoverOpacity
        scale: item.visualScale * item.hoverScale
        rotation: item.visualRotation + item.hoverRotation

        transform: Translate {
            x: item.xDanceOffset()
            y: item.yDanceOffset()
        }

        Behavior on opacity { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }
        Behavior on scale { NumberAnimation { duration: 130; easing.type: Easing.OutBack } }
        Behavior on rotation { NumberAnimation { duration: 90; easing.type: Easing.OutQuad } }
    }

    Rectangle {
        id: runningIndicator
        visible: item.running
        z: 30
        width: item.verticalDock ? 5 : 24
        height: item.verticalDock ? 24 : 5
        radius: 3
        color: item.active ? "#99ccff" : "#ffffff"
        opacity: 1.0

        anchors.horizontalCenter: item.verticalDock ? undefined : parent.horizontalCenter
        anchors.verticalCenter: item.verticalDock ? parent.verticalCenter : undefined
        anchors.bottom: item.verticalDock ? undefined : parent.bottom
        anchors.bottomMargin: item.verticalDock ? 0 : -1
        anchors.right: item.verticalDock ? parent.right : undefined
        anchors.rightMargin: item.verticalDock ? -1 : 0
    }

    ToolTip.visible: item.hovered
    ToolTip.delay: 500
    ToolTip.text: appName

    HoverHandler {
        id: iconHoverHandler
        target: item

        onHoveredChanged: {
            item.hoverChanged(hovered, item.danceIndex)
        }
    }

    MouseArea {
        id: mouse
        z: 40
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: function(event) {
            if (event.button === Qt.LeftButton) {
                item.launchRequested()
            } else if (event.button === Qt.RightButton) {
                menu.popup()
            }
        }
    }

    Menu {
        id: menu
        MenuItem { text: "Launch " + appName; enabled: appDesktopFile !== ""; onTriggered: item.launchRequested() }
        MenuSeparator {}
        MenuItem { text: "Remove from RemiDock"; onTriggered: item.removeRequested() }
    }
}
