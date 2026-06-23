cmake_minimum_required(VERSION 3.24)

project(RemiDock VERSION 0.4.12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Svg DBus Widgets)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup(REQUIRES 6.4)

qt_add_executable(RemiDock
    src/app/main.cpp

    src/core/RemiDockController.h
    src/core/RemiDockController.cpp

    src/models/PinnedAppsModel.h
    src/models/PinnedAppsModel.cpp
    src/models/AppCatalogModel.h
    src/models/AppCatalogModel.cpp

    src/platform/LayerShellManager.h
    src/platform/LayerShellManager.cpp

    src/media/MediaMonitor.h
    src/media/MediaMonitor.cpp
    src/media/AudioAnalyzer.h
    src/media/AudioAnalyzer.cpp

    src/ui/IconImageProvider.h
    src/ui/IconImageProvider.cpp
)

set_target_properties(RemiDock PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(RemiDock PRIVATE
    src/core
    src/models
    src/platform
    src/media
    src/ui
)

target_compile_definitions(RemiDock PRIVATE
    REMIDOCK_VERSION="${PROJECT_VERSION}"
)

qt_add_qml_module(RemiDock
    URI RemiDock
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        qml/Main.qml
        qml/dock/Dock.qml
        qml/dock/DockItem.qml
        qml/dock/DragHandle.qml
        qml/dock/MusicHandle.qml
        qml/dock/SettingsHandle.qml
        qml/settings/SettingsWindow.qml
        qml/hoverAnimations/SimpleHover.qml
        qml/hoverAnimations/ZoomHover.qml
        qml/hoverAnimations/FireHover.qml
        qml/hoverAnimations/FadeHover.qml
    RESOURCES
        assets/animations/fire.webp
        assets/animations/fire_source_apng.png
        assets/animations/fire.gif
        assets/animations/fire_spritesheet.png
        assets/animations/fire_user_apng.png
        assets/animations/fire_attached_apng.png
)

target_link_libraries(RemiDock PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    Qt6::DBus
    Qt6::Widgets
    LayerShellQt::Interface
)

install(TARGETS RemiDock DESTINATION bin)
install(FILES data/org.remisa.RemiDock.desktop DESTINATION share/applications)
install(FILES icon.png DESTINATION share/pixmaps RENAME remidock.png)
install(FILES LICENSE DESTINATION share/licenses/remidock)
install(FILES README.md DESTINATION share/doc/remidock)
