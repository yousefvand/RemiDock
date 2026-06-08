#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_qml_RemiDock_qml_Main_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_dock_Dock_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_dock_DockItem_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_dock_DragHandle_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_dock_MusicHandle_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_dock_SettingsHandle_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_settings_SettingsWindow_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_hoverAnimations_SimpleHover_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_hoverAnimations_ZoomHover_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_hoverAnimations_FireHover_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_RemiDock_qml_hoverAnimations_FadeHover_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/Main.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_Main_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/dock/Dock.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_dock_Dock_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/dock/DockItem.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_dock_DockItem_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/dock/DragHandle.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_dock_DragHandle_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/dock/MusicHandle.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_dock_MusicHandle_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/dock/SettingsHandle.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_dock_SettingsHandle_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/settings/SettingsWindow.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_settings_SettingsWindow_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/hoverAnimations/SimpleHover.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_hoverAnimations_SimpleHover_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/hoverAnimations/ZoomHover.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_hoverAnimations_ZoomHover_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/hoverAnimations/FireHover.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_hoverAnimations_FireHover_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/RemiDock/qml/hoverAnimations/FadeHover.qml"), &QmlCacheGeneratedCode::_qt_qml_RemiDock_qml_hoverAnimations_FadeHover_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_RemiDock)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_RemiDock))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_RemiDock)() {
    return 1;
}
