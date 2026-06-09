/****************************************************************************
** Meta object code from reading C++ file 'RemiDockController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/core/RemiDockController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RemiDockController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN18RemiDockControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto RemiDockController::qt_create_metaobjectdata<qt_meta_tag_ZN18RemiDockControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "RemiDockController",
        "iconSizeChanged",
        "",
        "dockHeightChanged",
        "autoHideChanged",
        "edgeChanged",
        "editModeChanged",
        "settingsVisibleChanged",
        "musicDanceEnabledChanged",
        "dockModeChanged",
        "hoverAnimationChanged",
        "dockFrameVisibleChanged",
        "layerShellNeedsReconfigure",
        "setIconSize",
        "value",
        "setDockHeight",
        "setAutoHide",
        "setEdge",
        "setEditMode",
        "setSettingsVisible",
        "setMusicDanceEnabled",
        "setDockMode",
        "setHoverAnimation",
        "setDockFrameVisible",
        "save",
        "cycleEdge",
        "setEdgeFromDrag",
        "dx",
        "dy",
        "openSettings",
        "closeSettings",
        "toggleSettings",
        "toggleEditMode",
        "toggleMusicDance",
        "iconSize",
        "dockHeight",
        "autoHide",
        "edge",
        "editMode",
        "settingsVisible",
        "musicDanceEnabled",
        "dockMode",
        "hoverAnimation",
        "dockFrameVisible"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'iconSizeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockHeightChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'autoHideChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'edgeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'editModeChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'settingsVisibleChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'musicDanceEnabledChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockModeChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'hoverAnimationChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dockFrameVisibleChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'layerShellNeedsReconfigure'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setIconSize'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'setDockHeight'
        QtMocHelpers::SlotData<void(int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'setAutoHide'
        QtMocHelpers::SlotData<void(bool)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Slot 'setEdge'
        QtMocHelpers::SlotData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'setEditMode'
        QtMocHelpers::SlotData<void(bool)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Slot 'setSettingsVisible'
        QtMocHelpers::SlotData<void(bool)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Slot 'setMusicDanceEnabled'
        QtMocHelpers::SlotData<void(bool)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Slot 'setDockMode'
        QtMocHelpers::SlotData<void(const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'setHoverAnimation'
        QtMocHelpers::SlotData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
        // Slot 'setDockFrameVisible'
        QtMocHelpers::SlotData<void(bool)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 14 },
        }}),
        // Slot 'save'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'cycleEdge'
        QtMocHelpers::MethodData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setEdgeFromDrag'
        QtMocHelpers::MethodData<void(qreal, qreal)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QReal, 27 }, { QMetaType::QReal, 28 },
        }}),
        // Method 'openSettings'
        QtMocHelpers::MethodData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'closeSettings'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleSettings'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleEditMode'
        QtMocHelpers::MethodData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'toggleMusicDance'
        QtMocHelpers::MethodData<void()>(33, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'iconSize'
        QtMocHelpers::PropertyData<int>(34, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
        // property 'dockHeight'
        QtMocHelpers::PropertyData<int>(35, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'autoHide'
        QtMocHelpers::PropertyData<bool>(36, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'edge'
        QtMocHelpers::PropertyData<QString>(37, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'editMode'
        QtMocHelpers::PropertyData<bool>(38, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'settingsVisible'
        QtMocHelpers::PropertyData<bool>(39, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
        // property 'musicDanceEnabled'
        QtMocHelpers::PropertyData<bool>(40, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 6),
        // property 'dockMode'
        QtMocHelpers::PropertyData<QString>(41, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'hoverAnimation'
        QtMocHelpers::PropertyData<QString>(42, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 8),
        // property 'dockFrameVisible'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<RemiDockController, qt_meta_tag_ZN18RemiDockControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject RemiDockController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18RemiDockControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18RemiDockControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18RemiDockControllerE_t>.metaTypes,
    nullptr
} };

void RemiDockController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<RemiDockController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->iconSizeChanged(); break;
        case 1: _t->dockHeightChanged(); break;
        case 2: _t->autoHideChanged(); break;
        case 3: _t->edgeChanged(); break;
        case 4: _t->editModeChanged(); break;
        case 5: _t->settingsVisibleChanged(); break;
        case 6: _t->musicDanceEnabledChanged(); break;
        case 7: _t->dockModeChanged(); break;
        case 8: _t->hoverAnimationChanged(); break;
        case 9: _t->dockFrameVisibleChanged(); break;
        case 10: _t->layerShellNeedsReconfigure(); break;
        case 11: _t->setIconSize((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->setDockHeight((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setAutoHide((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->setEdge((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->setEditMode((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->setSettingsVisible((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 17: _t->setMusicDanceEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 18: _t->setDockMode((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 19: _t->setHoverAnimation((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->setDockFrameVisible((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 21: _t->save(); break;
        case 22: _t->cycleEdge(); break;
        case 23: _t->setEdgeFromDrag((*reinterpret_cast<std::add_pointer_t<qreal>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qreal>>(_a[2]))); break;
        case 24: _t->openSettings(); break;
        case 25: _t->closeSettings(); break;
        case 26: _t->toggleSettings(); break;
        case 27: _t->toggleEditMode(); break;
        case 28: _t->toggleMusicDance(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::iconSizeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::dockHeightChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::autoHideChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::edgeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::editModeChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::settingsVisibleChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::musicDanceEnabledChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::dockModeChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::hoverAnimationChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::dockFrameVisibleChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (RemiDockController::*)()>(_a, &RemiDockController::layerShellNeedsReconfigure, 10))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->iconSize(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->dockHeight(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->autoHide(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->edge(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->editMode(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->settingsVisible(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->musicDanceEnabled(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->dockMode(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->hoverAnimation(); break;
        case 9: *reinterpret_cast<bool*>(_v) = _t->dockFrameVisible(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setIconSize(*reinterpret_cast<int*>(_v)); break;
        case 1: _t->setDockHeight(*reinterpret_cast<int*>(_v)); break;
        case 2: _t->setAutoHide(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setEdge(*reinterpret_cast<QString*>(_v)); break;
        case 4: _t->setEditMode(*reinterpret_cast<bool*>(_v)); break;
        case 5: _t->setSettingsVisible(*reinterpret_cast<bool*>(_v)); break;
        case 6: _t->setMusicDanceEnabled(*reinterpret_cast<bool*>(_v)); break;
        case 7: _t->setDockMode(*reinterpret_cast<QString*>(_v)); break;
        case 8: _t->setHoverAnimation(*reinterpret_cast<QString*>(_v)); break;
        case 9: _t->setDockFrameVisible(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *RemiDockController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RemiDockController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18RemiDockControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RemiDockController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 29;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void RemiDockController::iconSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RemiDockController::dockHeightChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RemiDockController::autoHideChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void RemiDockController::edgeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void RemiDockController::editModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void RemiDockController::settingsVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void RemiDockController::musicDanceEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void RemiDockController::dockModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void RemiDockController::hoverAnimationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void RemiDockController::dockFrameVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void RemiDockController::layerShellNeedsReconfigure()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
