/****************************************************************************
** Meta object code from reading C++ file 'presetparser.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/presetparser.h"
#include <QtNetwork/QSslPreSharedKeyAuthenticator>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'presetparser.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPresetParserENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSPresetParserENDCLASS = QtMocHelpers::stringData(
    "PresetParser",
    "QML.Element",
    "anonymous",
    "getOptions",
    "",
    "sendError",
    "error_msg",
    "mkwiiPathChanged",
    "dolphPathChanged",
    "writeToJson",
    "game_path",
    "save_location",
    "preset_name",
    "parseRiivoXML",
    "riivo_base_path",
    "display_name",
    "addOption",
    "option",
    "index",
    "collectOptions",
    "printHello",
    "savePreset",
    "save_path",
    "setGamePath",
    "mkwii_path",
    "setExecutablePath",
    "dolphin_executable",
    "runGame",
    "json_path",
    "mkwiiPath",
    "dolphPath"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPresetParserENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       1,   14, // classinfo
      13,   16, // methods
       2,  133, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   94,    4, 0x06,    3 /* Public */,
       5,    1,   95,    4, 0x06,    4 /* Public */,
       7,    0,   98,    4, 0x06,    6 /* Public */,
       8,    0,   99,    4, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    3,  100,    4, 0x0a,    8 /* Public */,
      13,    2,  107,    4, 0x0a,   12 /* Public */,
      16,    2,  112,    4, 0x0a,   15 /* Public */,
      19,    0,  117,    4, 0x0a,   18 /* Public */,
      20,    0,  118,    4, 0x0a,   19 /* Public */,
      21,    2,  119,    4, 0x0a,   20 /* Public */,
      23,    1,  124,    4, 0x0a,   23 /* Public */,
      25,    1,  127,    4, 0x0a,   25 /* Public */,
      27,    1,  130,    4, 0x0a,   27 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,
    QMetaType::Void, QMetaType::QUrl, QMetaType::QString,   14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QUrl,   15,   22,
    QMetaType::Void, QMetaType::QUrl,   24,
    QMetaType::Void, QMetaType::QUrl,   26,
    QMetaType::Void, QMetaType::QString,   28,

 // properties: name, type, flags, notifyId, revision
      29, QMetaType::QString, 0x00015103, uint(2), 0,
      30, QMetaType::QString, 0x00015103, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject PresetParser::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPresetParserENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPresetParserENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'mkwiiPath'
        QString,
        // property 'dolphPath'
        QString,
        // Q_OBJECT / Q_GADGET
        PresetParser,
        // method 'getOptions'
        void,
        // method 'sendError'
        void,
        QString,
        // method 'mkwiiPathChanged'
        void,
        // method 'dolphPathChanged'
        void,
        // method 'writeToJson'
        void,
        QString,
        QString,
        QString,
        // method 'parseRiivoXML'
        void,
        QUrl,
        QString,
        // method 'addOption'
        void,
        QString,
        int,
        // method 'collectOptions'
        void,
        // method 'printHello'
        void,
        // method 'savePreset'
        void,
        QString,
        QUrl,
        // method 'setGamePath'
        void,
        QUrl,
        // method 'setExecutablePath'
        void,
        QUrl,
        // method 'runGame'
        void,
        QString
    >,
    nullptr
} };

void PresetParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PresetParser *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->getOptions(); break;
        case 1: _t->sendError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->mkwiiPathChanged(); break;
        case 3: _t->dolphPathChanged(); break;
        case 4: _t->writeToJson((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3]))); break;
        case 5: _t->parseRiivoXML((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->addOption((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->collectOptions(); break;
        case 8: _t->printHello(); break;
        case 9: _t->savePreset((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[2]))); break;
        case 10: _t->setGamePath((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 11: _t->setExecutablePath((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 12: _t->runGame((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PresetParser::*)();
            if (_t _q_method = &PresetParser::getOptions; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PresetParser::*)(QString );
            if (_t _q_method = &PresetParser::sendError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PresetParser::*)();
            if (_t _q_method = &PresetParser::mkwiiPathChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PresetParser::*)();
            if (_t _q_method = &PresetParser::dolphPathChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    } else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<PresetParser *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->mkwiiPath(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->dolphPath(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<PresetParser *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMkwiiPath(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setDolphPath(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *PresetParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PresetParser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPresetParserENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PresetParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void PresetParser::getOptions()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PresetParser::sendError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PresetParser::mkwiiPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PresetParser::dolphPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
