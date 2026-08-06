/****************************************************************************
** Meta object code from reading C++ file 'VideoTrimmer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/VideoTrimmer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoTrimmer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12VideoTrimmerE_t {};
} // unnamed namespace

template <> constexpr inline auto VideoTrimmer::qt_create_metaobjectdata<qt_meta_tag_ZN12VideoTrimmerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "VideoTrimmer",
        "inPointChanged",
        "",
        "ms",
        "outPointChanged",
        "playheadMoved",
        "cutRequested",
        "positionMs"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'inPointChanged'
        QtMocHelpers::SignalData<void(qint64)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 3 },
        }}),
        // Signal 'outPointChanged'
        QtMocHelpers::SignalData<void(qint64)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 3 },
        }}),
        // Signal 'playheadMoved'
        QtMocHelpers::SignalData<void(qint64)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 3 },
        }}),
        // Signal 'cutRequested'
        QtMocHelpers::SignalData<void(qint64)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::LongLong, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<VideoTrimmer, qt_meta_tag_ZN12VideoTrimmerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject VideoTrimmer::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12VideoTrimmerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12VideoTrimmerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12VideoTrimmerE_t>.metaTypes,
    nullptr
} };

void VideoTrimmer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<VideoTrimmer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->inPointChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 1: _t->outPointChanged((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 2: _t->playheadMoved((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        case 3: _t->cutRequested((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (VideoTrimmer::*)(qint64 )>(_a, &VideoTrimmer::inPointChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTrimmer::*)(qint64 )>(_a, &VideoTrimmer::outPointChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTrimmer::*)(qint64 )>(_a, &VideoTrimmer::playheadMoved, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (VideoTrimmer::*)(qint64 )>(_a, &VideoTrimmer::cutRequested, 3))
            return;
    }
}

const QMetaObject *VideoTrimmer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoTrimmer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12VideoTrimmerE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VideoTrimmer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void VideoTrimmer::inPointChanged(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void VideoTrimmer::outPointChanged(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void VideoTrimmer::playheadMoved(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void VideoTrimmer::cutRequested(qint64 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
