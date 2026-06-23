#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDir>
#include <QStandardPaths>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include <QTimer>

#include <LayerShellQt/Shell>

#include "IconImageProvider.h"
#include "LayerShellManager.h"
#include "MediaMonitor.h"
#include "AudioAnalyzer.h"
#include "PinnedAppsModel.h"
#include "AppCatalogModel.h"
#include "RemiDockController.h"

#ifndef REMIDOCK_VERSION
#define REMIDOCK_VERSION "0.4.0"
#endif

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    QGuiApplication::setApplicationName("RemiDock");
    QGuiApplication::setApplicationVersion(QStringLiteral(REMIDOCK_VERSION));
    QGuiApplication::setOrganizationName("remisa");
    QGuiApplication::setDesktopFileName("org.remisa.RemiDock");

    LayerShellQt::Shell::useLayerShell();

    QStringList iconSearchPaths = QIcon::themeSearchPaths();
    const QStringList dataLocations = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    for (const QString &dataLocation : dataLocations)
        iconSearchPaths << dataLocation + QStringLiteral("/icons");
    iconSearchPaths << QDir::homePath() + QStringLiteral("/.icons");
    iconSearchPaths << QStringLiteral("/usr/share/icons");
    iconSearchPaths << QStringLiteral("/usr/local/share/icons");
    QIcon::setThemeSearchPaths(iconSearchPaths);
    QIcon::setFallbackThemeName(QStringLiteral("hicolor"));
    QIcon::setFallbackSearchPaths({
        QStringLiteral("/usr/share/pixmaps")
    });

    RemiDockController dockController;
    PinnedAppsModel pinnedApps;
    AppCatalogModel appCatalog;
    LayerShellManager layerShellManager(&dockController);
    MediaMonitor mediaMonitor;
    AudioAnalyzer audioAnalyzer;

    QQmlApplicationEngine engine;
    engine.addImageProvider("icon", new IconImageProvider);

    engine.rootContext()->setContextProperty("dockController", &dockController);
    engine.rootContext()->setContextProperty("pinnedAppsModel", &pinnedApps);
    engine.rootContext()->setContextProperty("appCatalogModel", &appCatalog);
    engine.rootContext()->setContextProperty("mediaMonitor", &mediaMonitor);
    engine.rootContext()->setContextProperty("audioAnalyzer", &audioAnalyzer);

    auto updateAudioAnalyzer = [&dockController, &mediaMonitor, &audioAnalyzer]() {
        audioAnalyzer.setEnabled(dockController.musicDanceEnabled() && mediaMonitor.playing());
    };

    QObject::connect(&dockController, &RemiDockController::musicDanceEnabledChanged, &app, updateAudioAnalyzer);
    QObject::connect(&mediaMonitor, &MediaMonitor::playingChanged, &app, updateAudioAnalyzer);
    updateAudioAnalyzer();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [&](QObject *object, const QUrl &) {
            auto *window = qobject_cast<QQuickWindow *>(object);
            if (!window)
                return;

            layerShellManager.setWindow(window);

            QTimer::singleShot(0, window, [&layerShellManager]() {
                layerShellManager.apply();
            });
        }
    );

    engine.loadFromModule("RemiDock", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
