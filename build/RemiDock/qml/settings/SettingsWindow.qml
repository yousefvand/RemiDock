import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Window {
    id: settingsWindow

    width: 820
    height: 610
    minimumWidth: 760
    minimumHeight: 560

    visible: false
    title: "RemiDock Settings"
    color: "#202028"

    flags: Qt.Window | Qt.WindowStaysOnTopHint

    property int pinnedCount: pinnedAppsModel.itemCount
    property string customIconPath: ""

    property bool canAddApp: pinnedCount >= 0 && pinnedAppsModel.canFitAdditionalApp(
                                 dockController.iconSize,
                                 Screen.width,
                                 Screen.height,
                                 dockController.edge)

    property bool canAddSeparator: pinnedCount >= 0 && pinnedAppsModel.canFitAdditionalSeparator(
                                       dockController.iconSize,
                                       Screen.width,
                                       Screen.height,
                                       dockController.edge)

    FileDialog {
        id: customIconDialog
        title: "Choose custom app icon"
        nameFilters: ["Image files (*.png *.jpg *.jpeg *.svg)", "All files (*)"]
        onAccepted: settingsWindow.customIconPath = selectedFile.toString()
    }

    Rectangle {
        anchors.fill: parent
        color: "#202028"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 34

                Label {
                    text: "RemiDock Settings"
                    font.pixelSize: 21
                    font.bold: true
                    color: "white"
                    Layout.fillWidth: true
                }

                Button {
                    text: "Close"
                    Layout.preferredWidth: 90
                    Layout.preferredHeight: 30
                    onClicked: settingsWindow.hide()
                }
            }

            Rectangle { Layout.fillWidth: true; height: 1; color: "#44ffffff" }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 154
                color: "transparent"
                clip: false

                ColumnLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 22
                    anchors.rightMargin: 22
                    anchors.topMargin: 4
                    anchors.bottomMargin: 2
                    spacing: 8

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        Button { text: "Bottom"; Layout.fillWidth: true; checkable: true; checked: dockController.edge === "bottom"; onClicked: dockController.edge = "bottom" }
                        Button { text: "Left"; Layout.fillWidth: true; checkable: true; checked: dockController.edge === "left"; onClicked: dockController.edge = "left" }
                        Button { text: "Top"; Layout.fillWidth: true; checkable: true; checked: dockController.edge === "top"; onClicked: dockController.edge = "top" }
                        Button { text: "Right"; Layout.fillWidth: true; checkable: true; checked: dockController.edge === "right"; onClicked: dockController.edge = "right" }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        Label { text: "Dock mode:"; color: "white"; Layout.preferredWidth: 92 }
                        Button { text: "Always show"; Layout.fillWidth: true; checkable: true; checked: dockController.dockMode === "always"; onClicked: dockController.dockMode = "always" }
                        Button { text: "Show on hover"; Layout.fillWidth: true; checkable: true; checked: dockController.dockMode === "hover"; onClicked: dockController.dockMode = "hover" }
                        Button { text: "Dodge windows"; Layout.fillWidth: true; checkable: true; checked: dockController.dockMode === "dodge"; onClicked: dockController.dockMode = "dodge" }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Label { text: "Icon: " + Math.round(iconSizeSlider.value); color: "white"; Layout.preferredWidth: 68 }
                        Slider {
                            id: iconSizeSlider
                            from: 28; to: 80; stepSize: 2
                            value: dockController.iconSize
                            live: false
                            Layout.fillWidth: true
                            onPressedChanged: if (!pressed) dockController.iconSize = Math.round(value)
                        }

                        Label { text: "Thickness: " + Math.round(thicknessSlider.value); color: "white"; Layout.preferredWidth: 112 }
                        Slider {
                            id: thicknessSlider
                            from: 52; to: 130; stepSize: 2
                            value: dockController.dockHeight
                            live: false
                            Layout.fillWidth: true
                            onPressedChanged: if (!pressed) dockController.dockHeight = Math.round(value)
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Label { text: "Hover animation:"; color: "white"; Layout.preferredWidth: 130 }
                        ComboBox {
                            id: hoverAnimationCombo
                            Layout.preferredWidth: 180
                            textRole: "label"
                            valueRole: "value"
                            model: [
                                { label: "Simple", value: "simple" },
                                { label: "Zoom", value: "zoom" },
                                { label: "Fire", value: "fire" },
                                { label: "Fade", value: "fade" }
                            ]

                            Component.onCompleted: currentIndex = indexOfValue(dockController.hoverAnimation)

                            onActivated: dockController.hoverAnimation = currentValue
                        }

                        Label {
                            text: "Drop new QML plugins into qml/hoverAnimations and add them to CMake/settings."
                            color: "#aaaaaa"
                            font.pixelSize: 10
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                GroupBox {
                    title: "Pinned dock items"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: 505

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 6
                        spacing: 6

                        RowLayout {
                            Layout.fillWidth: true
                            Label { text: "Order, remove, add separators, or add custom app."; color: "#cccccc"; Layout.fillWidth: true; elide: Text.ElideRight }
                            Button {
                                text: "Add separator"
                                Layout.preferredWidth: 124
                                enabled: settingsWindow.canAddSeparator
                                onClicked: addStatus.text = pinnedAppsModel.addSeparator() ? "Separator added." : "Could not add separator."
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            color: settingsWindow.canAddApp ? "#99ccff" : "#ff9999"
                            font.pixelSize: 11
                            wrapMode: Text.WordWrap
                            text: settingsWindow.canAddApp
                                  ? "Space is available for another app."
                                  : "Dock is full. Remove an item or reduce icon size before adding more."
                        }

                        Rectangle { Layout.fillWidth: true; height: 1; color: "#22ffffff" }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.minimumHeight: 90
                            clip: true

                            ListView {
                                id: pinnedList
                                width: parent.width
                                model: pinnedAppsModel
                                spacing: 2
                                clip: true

                                delegate: Rectangle {
                                    id: row
                                    required property int index
                                    required property string itemId
                                    required property string itemType
                                    required property string name
                                    required property string iconName
                                    required property string desktopFile

                                    width: pinnedList.width
                                    height: 32
                                    radius: 4
                                    color: row.itemType === "separator" ? "#24242b" : "#2d2d36"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: 6
                                        anchors.rightMargin: 6
                                        anchors.topMargin: 3
                                        anchors.bottomMargin: 3
                                        spacing: 5

                                        Item {
                                            Layout.preferredWidth: 22
                                            Layout.preferredHeight: 22
                                            Image { visible: row.itemType === "app"; anchors.centerIn: parent; source: "image://icon/" + row.iconName; width: 18; height: 18; smooth: true }
                                            Rectangle { visible: row.itemType === "separator"; anchors.centerIn: parent; width: 18; height: 2; radius: 1; color: "#88ffffff" }
                                        }

                                        Label { text: row.itemType === "separator" ? "Separator" : row.name; color: row.itemType === "separator" ? "#dddddd" : "white"; Layout.fillWidth: true; elide: Text.ElideRight }
                                        Label { visible: row.itemType === "app"; text: row.desktopFile; color: "#777777"; font.pixelSize: 9; Layout.preferredWidth: 95; elide: Text.ElideRight }
                                        Button { text: "Top"; Layout.preferredWidth: 48; Layout.preferredHeight: 24; enabled: row.index > 0; onClicked: pinnedAppsModel.moveToTop(row.itemId) }
                                        Button { text: "↑"; Layout.preferredWidth: 32; Layout.preferredHeight: 24; enabled: row.index > 0; onClicked: pinnedAppsModel.moveUp(row.itemId) }
                                        Button { text: "↓"; Layout.preferredWidth: 32; Layout.preferredHeight: 24; enabled: row.index < pinnedAppsModel.rowCount() - 1; onClicked: pinnedAppsModel.moveDown(row.itemId) }
                                        Button { text: "Bottom"; Layout.preferredWidth: 66; Layout.preferredHeight: 24; enabled: row.index < pinnedAppsModel.rowCount() - 1; onClicked: pinnedAppsModel.moveToBottom(row.itemId) }
                                        Button { text: "Remove"; Layout.preferredWidth: 72; Layout.preferredHeight: 24; onClicked: pinnedAppsModel.removeById(row.itemId) }
                                    }
                                }
                            }
                        }

                        Rectangle { Layout.fillWidth: true; height: 1; color: "#22ffffff" }

                        Label { text: "Add custom application"; color: "white"; font.bold: true; Layout.fillWidth: true }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 6

                            TextField {
                                id: customExec
                                Layout.fillWidth: true
                                Layout.preferredHeight: 30
                                placeholderText: "Executable path, e.g. /opt/MyApp/myapp"
                                selectByMouse: true
                            }

                            Button {
                                text: "Icon"
                                Layout.preferredWidth: 60
                                Layout.preferredHeight: 30
                                onClicked: customIconDialog.open()
                            }

                            Button {
                                text: "Add"
                                Layout.preferredWidth: 58
                                Layout.preferredHeight: 30
                                enabled: settingsWindow.canAddApp && customExec.text.trim().length > 0
                                onClicked: {
                                    if (pinnedAppsModel.addCustomApplication(customExec.text, settingsWindow.customIconPath)) {
                                        addStatus.color = "#99ff99"
                                        addStatus.text = "Custom application added."
                                        customExec.text = ""
                                        settingsWindow.customIconPath = ""
                                    } else {
                                        addStatus.color = "#ff9999"
                                        addStatus.text = "Could not add custom app. Check executable path."
                                    }
                                }
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            text: settingsWindow.customIconPath.length > 0 ? settingsWindow.customIconPath.replace("file://", "") : "No icon selected (.png/.svg optional)"
                            color: "#aaaaaa"
                            font.pixelSize: 10
                            elide: Text.ElideMiddle
                        }
                    }
                }

                GroupBox {
                    title: "Add installed app"
                    Layout.fillHeight: true
                    Layout.preferredWidth: 292
                    Layout.minimumWidth: 280

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 6
                        spacing: 6

                        TextField {
                            id: appSearch
                            Layout.fillWidth: true
                            placeholderText: "Type installed app name..."
                            selectByMouse: true
                            onTextChanged: appCatalogModel.searchText = text
                        }

                        Label {
                            id: addStatus
                            Layout.fillWidth: true
                            color: settingsWindow.canAddApp ? "#cccccc" : "#ff9999"
                            font.pixelSize: 11
                            wrapMode: Text.WordWrap
                            text: settingsWindow.canAddApp ? "Search installed apps and click Add." : "Dock is full. Remove an item or reduce icon size."
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true

                            ListView {
                                id: appList
                                width: parent.width
                                model: appCatalogModel
                                spacing: 4
                                clip: true

                                delegate: Rectangle {
                                    id: appRow
                                    required property string name
                                    required property string iconName
                                    required property string desktopFile

                                    width: appList.width
                                    height: 42
                                    radius: 6
                                    color: "#2d2d36"

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.margins: 6
                                        spacing: 6

                                        Image { source: "image://icon/" + appRow.iconName; width: 22; height: 22; smooth: true }
                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: 0
                                            Label { text: appRow.name; color: "white"; Layout.fillWidth: true; elide: Text.ElideRight }
                                            Label { text: appRow.desktopFile; color: "#888888"; font.pixelSize: 9; Layout.fillWidth: true; elide: Text.ElideRight }
                                        }
                                        Button {
                                            text: "Add"
                                            Layout.preferredWidth: 52
                                            Layout.preferredHeight: 28
                                            enabled: settingsWindow.canAddApp
                                            onClicked: {
                                                if (pinnedAppsModel.addDesktopFile(appRow.desktopFile)) {
                                                    addStatus.color = "#99ff99"
                                                    addStatus.text = "Added " + appRow.name + "."
                                                } else {
                                                    addStatus.color = "#ff9999"
                                                    addStatus.text = "Could not add " + appRow.name + "."
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
