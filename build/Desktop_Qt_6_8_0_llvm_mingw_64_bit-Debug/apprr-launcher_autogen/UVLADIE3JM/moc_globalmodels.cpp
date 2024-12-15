/****************************************************************************
** Meta object code from reading C++ file 'globalmodels.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/globalmodels.h"
#include <QtNetwork/QSslPreSharedKeyAuthenticator>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'globalmodels.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSGlobalModelsENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSGlobalModelsENDCLASS = QtMocHelpers::stringData(
    "GlobalModels",
    "QML.Singleton",
    "true",
    "QML.Element",
    "auto",
    "gamePresetModelChanged",
    "",
    "presetParserChanged",
    "optionModelChanged",
    "hello",
    "presetParser",
    "PresetParser*",
    "gamePresetModel",
    "GamePresetModel*",
    "optionModel",
    "OptionModel*"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGlobalModelsENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       2,   14, // classinfo
       4,   18, // methods
       3,   46, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // classinfo: key, value
       1,    2,
       3,    4,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   42,    6, 0x06,    4 /* Public */,
       7,    0,   43,    6, 0x06,    5 /* Public */,
       8,    0,   44,    6, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   45,    6, 0x0a,    7 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
      10, 0x80000000 | 11, 0x0001510b, uint(1), 0,
      12, 0x80000000 | 13, 0x0001510b, uint(0), 0,
      14, 0x80000000 | 15, 0x0001510b, uint(2), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject GlobalModels::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSGlobalModelsENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGlobalModelsENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'presetParser'
        PresetParser*,
        // property 'gamePresetModel'
        GamePresetModel*,
        // property 'optionModel'
        OptionModel*,
        // Q_OBJECT / Q_GADGET
        GlobalModels,
        // method 'gamePresetModelChanged'
        void,
        // method 'presetParserChanged'
        void,
        // method 'optionModelChanged'
        void,
        // method 'hello'
        void
    >,
    nullptr
} };

void GlobalModels::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GlobalModels *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->gamePresetModelChanged(); break;
        case 1: _t->presetParserChanged(); break;
        case 2: _t->optionModelChanged(); break;
        case 3: _t->hello(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GlobalModels::*)();
            if (_t _q_method = &GlobalModels::gamePresetModelChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GlobalModels::*)();
            if (_t _q_method = &GlobalModels::presetParserChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GlobalModels::*)();
            if (_t _q_method = &GlobalModels::optionModelChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GamePresetModel* >(); break;
        case 2:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< OptionModel* >(); break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PresetParser* >(); break;
        }
    }  else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<GlobalModels *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< PresetParser**>(_v) = _t->presetParser(); break;
        case 1: *reinterpret_cast< GamePresetModel**>(_v) = _t->gamePresetModel(); break;
        case 2: *reinterpret_cast< OptionModel**>(_v) = _t->optionModel(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<GlobalModels *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setPresetParser(*reinterpret_cast< PresetParser**>(_v)); break;
        case 1: _t->setGamePresetModel(*reinterpret_cast< GamePresetModel**>(_v)); break;
        case 2: _t->setOptionModel(*reinterpret_cast< OptionModel**>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *GlobalModels::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlobalModels::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGlobalModelsENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GlobalModels::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void GlobalModels::gamePresetModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GlobalModels::presetParserChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GlobalModels::optionModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
