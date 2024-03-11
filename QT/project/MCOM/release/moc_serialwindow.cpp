/****************************************************************************
** Meta object code from reading C++ file 'serialwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MCOM/serialwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.2. It"
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
struct qt_meta_stringdata_CLASSserialwindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSserialwindowENDCLASS = QtMocHelpers::stringData(
    "serialwindow",
    "on_pushButton_clicked",
    "",
    "on_enter_bt_clicked",
    "on_mc_startBt_clicked",
    "on_mt_stopBt_clicked",
    "sendKpCommand",
    "sendKiCommand",
    "sendKcCommand",
    "sendIdCommand",
    "sendIqCommand",
    "onReadSerialData"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSserialwindowENDCLASS_t {
    uint offsetsAndSizes[24];
    char stringdata0[13];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[20];
    char stringdata4[22];
    char stringdata5[21];
    char stringdata6[14];
    char stringdata7[14];
    char stringdata8[14];
    char stringdata9[14];
    char stringdata10[14];
    char stringdata11[17];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSserialwindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSserialwindowENDCLASS_t qt_meta_stringdata_CLASSserialwindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "serialwindow"
        QT_MOC_LITERAL(13, 21),  // "on_pushButton_clicked"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 19),  // "on_enter_bt_clicked"
        QT_MOC_LITERAL(56, 21),  // "on_mc_startBt_clicked"
        QT_MOC_LITERAL(78, 20),  // "on_mt_stopBt_clicked"
        QT_MOC_LITERAL(99, 13),  // "sendKpCommand"
        QT_MOC_LITERAL(113, 13),  // "sendKiCommand"
        QT_MOC_LITERAL(127, 13),  // "sendKcCommand"
        QT_MOC_LITERAL(141, 13),  // "sendIdCommand"
        QT_MOC_LITERAL(155, 13),  // "sendIqCommand"
        QT_MOC_LITERAL(169, 16)   // "onReadSerialData"
    },
    "serialwindow",
    "on_pushButton_clicked",
    "",
    "on_enter_bt_clicked",
    "on_mc_startBt_clicked",
    "on_mt_stopBt_clicked",
    "sendKpCommand",
    "sendKiCommand",
    "sendKcCommand",
    "sendIdCommand",
    "sendIqCommand",
    "onReadSerialData"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSserialwindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    1 /* Private */,
       3,    0,   75,    2, 0x08,    2 /* Private */,
       4,    0,   76,    2, 0x08,    3 /* Private */,
       5,    0,   77,    2, 0x08,    4 /* Private */,
       6,    0,   78,    2, 0x08,    5 /* Private */,
       7,    0,   79,    2, 0x08,    6 /* Private */,
       8,    0,   80,    2, 0x08,    7 /* Private */,
       9,    0,   81,    2, 0x08,    8 /* Private */,
      10,    0,   82,    2, 0x08,    9 /* Private */,
      11,    0,   83,    2, 0x08,   10 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject serialwindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSserialwindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSserialwindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSserialwindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<serialwindow, std::true_type>,
        // method 'on_pushButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_enter_bt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_mc_startBt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_mt_stopBt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendKpCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendKiCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendKcCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendIdCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendIqCommand'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReadSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void serialwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<serialwindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_enter_bt_clicked(); break;
        case 2: _t->on_mc_startBt_clicked(); break;
        case 3: _t->on_mt_stopBt_clicked(); break;
        case 4: _t->sendKpCommand(); break;
        case 5: _t->sendKiCommand(); break;
        case 6: _t->sendKcCommand(); break;
        case 7: _t->sendIdCommand(); break;
        case 8: _t->sendIqCommand(); break;
        case 9: _t->onReadSerialData(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *serialwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *serialwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSserialwindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int serialwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
