/****************************************************************************
** Meta object code from reading C++ file 'PinnedAppsModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/models/PinnedAppsModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PinnedAppsModel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN15PinnedAppsModelE_t {};
} // unnamed namespace

template <> constexpr inline auto PinnedAppsModel::qt_create_metaobjectdata<qt_meta_tag_ZN15PinnedAppsModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PinnedAppsModel",
        "itemCountChanged",
        "",
        "launch",
        "index",
        "addDesktopFile",
        "desktopFile",
        "addCustomApplication",
        "executablePath",
        "iconPath",
        "addSeparator",
        "removeItem",
        "removeById",
        "id",
        "moveItem",
        "from",
        "to",
        "moveUp",
        "moveDown",
        "moveToTop",
        "moveToBottom",
        "canFitAdditionalApp",
        "iconSize",
        "screenWidth",
        "screenHeight",
        "edge",
        "canFitAdditionalSeparator",
        "estimatedDockExtentWithAdditional",
        "itemType",
        "maximumDockExtent",
        "resetDefaults",
        "refreshRunningState",
        "itemCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'itemCountChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'launch'
        QtMocHelpers::MethodData<void(int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'addDesktopFile'
        QtMocHelpers::MethodData<bool(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'addCustomApplication'
        QtMocHelpers::MethodData<bool(const QString &, const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 9 },
        }}),
        // Method 'addSeparator'
        QtMocHelpers::MethodData<bool()>(10, 2, QMC::AccessPublic, QMetaType::Bool),
        // Method 'removeItem'
        QtMocHelpers::MethodData<void(int)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Method 'removeById'
        QtMocHelpers::MethodData<void(const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'moveItem'
        QtMocHelpers::MethodData<void(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { QMetaType::Int, 16 },
        }}),
        // Method 'moveUp'
        QtMocHelpers::MethodData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'moveDown'
        QtMocHelpers::MethodData<void(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'moveToTop'
        QtMocHelpers::MethodData<void(const QString &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'moveToBottom'
        QtMocHelpers::MethodData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'canFitAdditionalApp'
        QtMocHelpers::MethodData<bool(int, int, int, const QString &) const>(21, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 22 }, { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::QString, 25 },
        }}),
        // Method 'canFitAdditionalSeparator'
        QtMocHelpers::MethodData<bool(int, int, int, const QString &) const>(26, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 22 }, { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::QString, 25 },
        }}),
        // Method 'estimatedDockExtentWithAdditional'
        QtMocHelpers::MethodData<int(const QString &, int, const QString &) const>(27, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 28 }, { QMetaType::Int, 22 }, { QMetaType::QString, 25 },
        }}),
        // Method 'maximumDockExtent'
        QtMocHelpers::MethodData<int(int, int, const QString &) const>(29, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 23 }, { QMetaType::Int, 24 }, { QMetaType::QString, 25 },
        }}),
        // Method 'resetDefaults'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'refreshRunningState'
        QtMocHelpers::MethodData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'itemCount'
        QtMocHelpers::PropertyData<int>(32, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PinnedAppsModel, qt_meta_tag_ZN15PinnedAppsModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PinnedAppsModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PinnedAppsModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PinnedAppsModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15PinnedAppsModelE_t>.metaTypes,
    nullptr
} };

void PinnedAppsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PinnedAppsModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->itemCountChanged(); break;
        case 1: _t->launch((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: { bool _r = _t->addDesktopFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->addCustomApplication((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->addSeparator();
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->removeItem((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->removeById((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->moveItem((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->moveUp((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->moveDown((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 10: _t->moveToTop((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->moveToBottom((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: { bool _r = _t->canFitAdditionalApp((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 13: { bool _r = _t->canFitAdditionalSeparator((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 14: { int _r = _t->estimatedDockExtentWithAdditional((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 15: { int _r = _t->maximumDockExtent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast<int*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->resetDefaults(); break;
        case 17: _t->refreshRunningState(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PinnedAppsModel::*)()>(_a, &PinnedAppsModel::itemCountChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->itemCount(); break;
        default: break;
        }
    }
}

const QMetaObject *PinnedAppsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PinnedAppsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PinnedAppsModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int PinnedAppsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 18;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void PinnedAppsModel::itemCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
