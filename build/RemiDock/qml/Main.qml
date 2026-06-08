import QtQuick
import QtQuick.Controls
import RemiDock
import "dock"
import "settings"

Window {
    id: root

    visible: false
    color: "transparent"

    flags: Qt.FramelessWindowHint
         | Qt.NoDropShadowWindowHint

    width: dock.width
    height: dock.height
    minimumWidth: dock.width
    minimumHeight: dock.height
    maximumWidth: dock.width
    maximumHeight: dock.height

    Dock {
        id: dock
        edge: dockController.edge
        x: 0
        y: 0
    }

    SettingsWindow {
        id: settingsWindow
        visible: dockController.settingsVisible

        onClosing: function(close) {
            dockController.closeSettings()
        }
    }
}
