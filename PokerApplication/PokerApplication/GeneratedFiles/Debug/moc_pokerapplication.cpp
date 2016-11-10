/****************************************************************************
** Meta object code from reading C++ file 'pokerapplication.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../pokerapplication.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pokerapplication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PokerApplication_t {
    QByteArrayData data[5];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PokerApplication_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PokerApplication_t qt_meta_stringdata_PokerApplication = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PokerApplication"
QT_MOC_LITERAL(1, 17, 11), // "ConnectToDb"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "webChannelTest"
QT_MOC_LITERAL(4, 45, 4) // "text"

    },
    "PokerApplication\0ConnectToDb\0\0"
    "webChannelTest\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PokerApplication[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x02 /* Public */,
       3,    1,   30,    2, 0x02 /* Public */,
       3,    0,   33,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Int,

       0        // eod
};

void PokerApplication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PokerApplication *_t = static_cast<PokerApplication *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->ConnectToDb();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: _t->webChannelTest((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: { int _r = _t->webChannelTest();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject PokerApplication::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PokerApplication.data,
      qt_meta_data_PokerApplication,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PokerApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PokerApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PokerApplication.stringdata0))
        return static_cast<void*>(const_cast< PokerApplication*>(this));
    return QObject::qt_metacast(_clname);
}

int PokerApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
