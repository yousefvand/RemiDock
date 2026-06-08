/****************************************************************************
** Meta object code from reading C++ file 'AudioAnalyzer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/media/AudioAnalyzer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioAnalyzer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13AudioAnalyzerE_t {};
} // unnamed namespace

template <> constexpr inline auto AudioAnalyzer::qt_create_metaobjectdata<qt_meta_tag_ZN13AudioAnalyzerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "AudioAnalyzer",
        "availableChanged",
        "",
        "runningChanged",
        "levelChanged",
        "beatChanged",
        "fastChanged",
        "slowChanged",
        "pulseChanged",
        "setEnabled",
        "enabled",
        "start",
        "stop",
        "readAudio",
        "processFrame",
        "handleProcessFinished",
        "exitCode",
        "QProcess::ExitStatus",
        "exitStatus",
        "available",
        "running",
        "level",
        "beat",
        "fast",
        "slow",
        "pulse"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'availableChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'runningChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'levelChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'beatChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fastChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'slowChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'pulseChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setEnabled'
        QtMocHelpers::SlotData<void(bool)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 10 },
        }}),
        // Slot 'start'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'readAudio'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'processFrame'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleProcessFinished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 16 }, { 0x80000000 | 17, 18 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'available'
        QtMocHelpers::PropertyData<bool>(19, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'running'
        QtMocHelpers::PropertyData<bool>(20, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'level'
        QtMocHelpers::PropertyData<qreal>(21, QMetaType::QReal, QMC::DefaultPropertyFlags, 2),
        // property 'beat'
        QtMocHelpers::PropertyData<qreal>(22, QMetaType::QReal, QMC::DefaultPropertyFlags, 3),
        // property 'fast'
        QtMocHelpers::PropertyData<qreal>(23, QMetaType::QReal, QMC::DefaultPropertyFlags, 4),
        // property 'slow'
        QtMocHelpers::PropertyData<qreal>(24, QMetaType::QReal, QMC::DefaultPropertyFlags, 5),
        // property 'pulse'
        QtMocHelpers::PropertyData<qreal>(25, QMetaType::QReal, QMC::DefaultPropertyFlags, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<AudioAnalyzer, qt_meta_tag_ZN13AudioAnalyzerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject AudioAnalyzer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AudioAnalyzerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AudioAnalyzerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13AudioAnalyzerE_t>.metaTypes,
    nullptr
} };

void AudioAnalyzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AudioAnalyzer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->availableChanged(); break;
        case 1: _t->runningChanged(); break;
        case 2: _t->levelChanged(); break;
        case 3: _t->beatChanged(); break;
        case 4: _t->fastChanged(); break;
        case 5: _t->slowChanged(); break;
        case 6: _t->pulseChanged(); break;
        case 7: _t->setEnabled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->start(); break;
        case 9: _t->stop(); break;
        case 10: _t->readAudio(); break;
        case 11: _t->processFrame(); break;
        case 12: _t->handleProcessFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::availableChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::runningChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::levelChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::beatChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::fastChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::slowChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (AudioAnalyzer::*)()>(_a, &AudioAnalyzer::pulseChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->available(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->running(); break;
        case 2: *reinterpret_cast<qreal*>(_v) = _t->level(); break;
        case 3: *reinterpret_cast<qreal*>(_v) = _t->beat(); break;
        case 4: *reinterpret_cast<qreal*>(_v) = _t->fast(); break;
        case 5: *reinterpret_cast<qreal*>(_v) = _t->slow(); break;
        case 6: *reinterpret_cast<qreal*>(_v) = _t->pulse(); break;
        default: break;
        }
    }
}

const QMetaObject *AudioAnalyzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioAnalyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13AudioAnalyzerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AudioAnalyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AudioAnalyzer::availableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AudioAnalyzer::runningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AudioAnalyzer::levelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AudioAnalyzer::beatChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AudioAnalyzer::fastChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void AudioAnalyzer::slowChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AudioAnalyzer::pulseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
