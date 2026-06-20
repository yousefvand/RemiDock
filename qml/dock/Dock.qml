import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: dock

    property string edge: "bottom"
    property bool vertical: edge === "left" || edge === "right"
    property bool iconsShouldDance: dockController.musicDanceEnabled && mediaMonitor.playing && audioAnalyzer.running
    property bool hoverActive: false
    property int hoveredIconIndex: -1
    property bool edgePointerInside: false
    property bool hiddenByMode: dockController.dockMode === "hover" && !hoverActive

    readonly property int hiddenOffsetX: hiddenByMode ? (edge === "left" ? -Math.max(0, backgroundWidth - 8) : (edge === "right" ? Math.max(0, backgroundWidth - 8) : 0)) : 0
    readonly property int hiddenOffsetY: hiddenByMode ? (edge === "top" ? -Math.max(0, backgroundHeight - 8) : (edge === "bottom" ? Math.max(0, backgroundHeight - 8) : 0)) : 0

    readonly property int handleSize: 28
    readonly property int outerPadding: 10
    readonly property int handleGap: 8
    readonly property int danceMargin: 38
    readonly property int maxHorizontalWidth: Math.max(220, Screen.width - 48)
    readonly property int maxVerticalHeight: Math.max(220, Screen.height - 48)

    readonly property int backgroundWidth: vertical
                                           ? dockController.dockHeight
                                           : Math.min(horizontalIconsRow.implicitWidth + handleSize * 2 + handleGap * 2 + outerPadding * 2,
                                                      maxHorizontalWidth)

    readonly property int backgroundHeight: vertical
                                            ? Math.min(verticalIconsColumn.implicitHeight + handleSize * 2 + handleGap * 2 + outerPadding * 2,
                                                       maxVerticalHeight)
                                            : dockController.dockHeight

    /*
       The layer-shell window is transparent and a bit larger than the black
       dock background. This extra transparent area is where dancing icons are
       allowed to move. The black rounded rectangle remains normal size.
    */
    width: vertical ? backgroundWidth + danceMargin : backgroundWidth
    height: vertical ? backgroundHeight : backgroundHeight + danceMargin
    clip: false

    Timer {
        id: hoverHideTimer
        interval: 650
        repeat: false
        onTriggered: {
            if (dockController.dockMode === "hover")
                dock.hoverActive = false
        }
    }

    /*
       Dock modes:
       - Always show: QML stays visible on top.
       - Show on hover: QML hides/reveals on edge hover.
       - Dodge windows: QML stays visible, but the layer-shell window is moved
         to the bottom layer so normal/fullscreen application windows cover it.
         This means the dock does not pop up over VLC controls.
    */
    Connections {
        target: dockController

        function onDockModeChanged() {
            hoverHideTimer.stop()
            dock.hoverActive = dockController.dockMode !== "hover"
        }
    }

    Component.onCompleted: {
        dock.hoverActive = dockController.dockMode !== "hover"
    }

    HoverHandler {
        id: dockHoverHandler
        target: dock

        onHoveredChanged: {
            if (dockController.dockMode === "hover") {
                if (hovered) {
                    hoverHideTimer.stop()
                    dock.hoverActive = true
                } else {
                    hoverHideTimer.restart()
                }
            } else {
                hoverHideTimer.stop()
                dock.hoverActive = true
            }
        }
    }

    Rectangle {
        id: revealStrip
        visible: dockController.dockMode === "hover" && dock.hiddenByMode
        z: 90
        color: "#55ffffff"
        opacity: 0.65
        radius: 3
        width: dock.vertical ? 6 : Math.min(96, dock.backgroundWidth * 0.35)
        height: dock.vertical ? Math.min(96, dock.backgroundHeight * 0.35) : 6
        anchors.horizontalCenter: dock.vertical ? undefined : parent.horizontalCenter
        anchors.verticalCenter: dock.vertical ? parent.verticalCenter : undefined
        anchors.left: dock.edge === "left" ? parent.left : undefined
        anchors.right: dock.edge === "right" ? parent.right : undefined
        anchors.top: dock.edge === "top" ? parent.top : undefined
        anchors.bottom: dock.edge === "bottom" ? parent.bottom : undefined
    }

    Rectangle {
        id: dockBackground

        width: dock.backgroundWidth
        height: dock.backgroundHeight
        radius: 24
        color: dockController.dockFrameVisible ? "#dd202028" : "transparent"
        border.color: dockController.dockFrameVisible ? "#55ffffff" : "transparent"
        border.width: dockController.dockFrameVisible ? 1 : 0
        clip: false

        x: {
            var base = 0
            if (dock.edge === "right")
                base = dock.danceMargin
            return base + dock.hiddenOffsetX
        }

        y: {
            var base = 0
            if (dock.edge === "bottom")
                base = dock.danceMargin
            return base + dock.hiddenOffsetY
        }

        opacity: dock.hiddenByMode ? 0.08 : 1.0

        Behavior on x { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        Behavior on y { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
        Behavior on opacity { NumberAnimation { duration: 120 } }
    }

    RowLayout {
        id: horizontalLayout
        visible: !vertical
        anchors.fill: dockBackground
        anchors.margins: outerPadding
        spacing: handleGap
        clip: false
        z: 10

        MusicHandle {
            Layout.preferredWidth: handleSize
            Layout.preferredHeight: handleSize
            Layout.alignment: Qt.AlignVCenter
            z: 50
        }

        Flickable {
            id: horizontalIconArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: false
            interactive: contentWidth > width
            boundsBehavior: Flickable.StopAtBounds
            contentWidth: horizontalIconsRow.implicitWidth
            contentHeight: height
            z: 20

            RowLayout {
                id: horizontalIconsRow
                height: horizontalIconArea.height
                spacing: 8
                clip: false

                Repeater {
                    model: pinnedAppsModel

                    delegate: Item {
                        required property int index
                        required property string itemType
                        required property string name
                        required property string iconName
                        required property string desktopFile
                        required property bool running
                        required property bool active

                        Layout.alignment: Qt.AlignVCenter

                        width: itemType === "separator" ? 12 : dockController.iconSize + 16
                        height: dockController.iconSize + 16
                        clip: false

                        Rectangle {
                            visible: itemType === "separator"
                            width: 2
                            height: Math.max(22, dockController.iconSize - 10)
                            radius: 1
                            color: "#66ffffff"
                            anchors.centerIn: parent
                        }

                        DockItem {
                            visible: itemType === "app"
                            anchors.fill: parent
                            clip: false

                            appName: name
                            appIcon: iconName
                            appDesktopFile: desktopFile
                            appIndex: index
                            running: running
                            active: active
                            verticalDock: false
                            dockEdge: dock.edge
                            dancing: dock.iconsShouldDance
                            danceIndex: index
                            audioLevel: audioAnalyzer.level
                            audioBeat: audioAnalyzer.beat
                            audioFast: audioAnalyzer.fast
                            audioSlow: audioAnalyzer.slow
                            audioPulse: audioAnalyzer.pulse
                            waveStrength: dock.hoveredIconIndex === index ? 1.0
                                          : Math.abs(dock.hoveredIconIndex - index) === 1 ? 0.5
                                          : 0.0

                            onHoverChanged: function(isHovered, itemIndex) {
                                if (isHovered)
                                    dock.hoveredIconIndex = itemIndex
                                else if (dock.hoveredIconIndex === itemIndex)
                                    dock.hoveredIconIndex = -1
                            }

                            onLaunchRequested: pinnedAppsModel.launch(index)
                            onRemoveRequested: pinnedAppsModel.removeItem(index)
                        }
                    }
                }
            }
        }

        SettingsHandle {
            Layout.preferredWidth: handleSize
            Layout.preferredHeight: handleSize
            Layout.alignment: Qt.AlignVCenter
            z: 50
        }
    }

    ColumnLayout {
        id: verticalLayout
        visible: vertical
        anchors.fill: dockBackground
        anchors.margins: outerPadding
        spacing: handleGap
        clip: false
        z: 10

        MusicHandle {
            Layout.preferredWidth: handleSize
            Layout.preferredHeight: handleSize
            Layout.alignment: Qt.AlignHCenter
            z: 50
        }

        Flickable {
            id: verticalIconArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: false
            interactive: contentHeight > height
            boundsBehavior: Flickable.StopAtBounds
            contentWidth: width
            contentHeight: verticalIconsColumn.implicitHeight
            z: 20

            ColumnLayout {
                id: verticalIconsColumn
                width: verticalIconArea.width
                spacing: 8
                clip: false

                Repeater {
                    model: pinnedAppsModel

                    delegate: Item {
                        required property int index
                        required property string itemType
                        required property string name
                        required property string iconName
                        required property string desktopFile
                        required property bool running
                        required property bool active

                        Layout.alignment: Qt.AlignHCenter

                        width: dockController.iconSize + 16
                        height: itemType === "separator" ? 12 : dockController.iconSize + 16
                        clip: false

                        Rectangle {
                            visible: itemType === "separator"
                            width: Math.max(22, dockController.iconSize - 10)
                            height: 2
                            radius: 1
                            color: "#66ffffff"
                            anchors.centerIn: parent
                        }

                        DockItem {
                            visible: itemType === "app"
                            anchors.fill: parent
                            clip: false

                            appName: name
                            appIcon: iconName
                            appDesktopFile: desktopFile
                            appIndex: index
                            running: running
                            active: active
                            verticalDock: true
                            dockEdge: dock.edge
                            dancing: dock.iconsShouldDance
                            danceIndex: index
                            audioLevel: audioAnalyzer.level
                            audioBeat: audioAnalyzer.beat
                            audioFast: audioAnalyzer.fast
                            audioSlow: audioAnalyzer.slow
                            audioPulse: audioAnalyzer.pulse
                            waveStrength: dock.hoveredIconIndex === index ? 1.0
                                          : Math.abs(dock.hoveredIconIndex - index) === 1 ? 0.5
                                          : 0.0

                            onHoverChanged: function(isHovered, itemIndex) {
                                if (isHovered)
                                    dock.hoveredIconIndex = itemIndex
                                else if (dock.hoveredIconIndex === itemIndex)
                                    dock.hoveredIconIndex = -1
                            }

                            onLaunchRequested: pinnedAppsModel.launch(index)
                            onRemoveRequested: pinnedAppsModel.removeItem(index)
                        }
                    }
                }
            }
        }

        SettingsHandle {
            Layout.preferredWidth: handleSize
            Layout.preferredHeight: handleSize
            Layout.alignment: Qt.AlignHCenter
            z: 50
        }
    }
}
