/****************************************************************************
** Meta object code from reading C++ file 'serialwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MCOM/serialwindow.h"
#include <QtGui/qtextcursor.h>
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
    "on_mc_startBt_clicked",
    "",
    "on_mt_stopBt_clicked",
    "onReadSerialData",
    "processdata",
    "data",
    "on_debug_bt_clicked",
    "onDataReceivedFromB",
    "on_enseriBt_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSserialwindowENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[13];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[17];
    char stringdata5[12];
    char stringdata6[5];
    char stringdata7[20];
    char stringdata8[20];
    char stringdata9[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSserialwindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSserialwindowENDCLASS_t qt_meta_stringdata_CLASSserialwindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "serialwindow"
        QT_MOC_LITERAL(13, 21),  // "on_mc_startBt_clicked"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 20),  // "on_mt_stopBt_clicked"
        QT_MOC_LITERAL(57, 16),  // "onReadSerialData"
        QT_MOC_LITERAL(74, 11),  // "processdata"
        QT_MOC_LITERAL(86, 4),  // "data"
        QT_MOC_LITERAL(91, 19),  // "on_debug_bt_clicked"
        QT_MOC_LITERAL(111, 19),  // "onDataReceivedFromB"
        QT_MOC_LITERAL(131, 19)   // "on_enseriBt_clicked"
    },
    "serialwindow",
    "on_mc_startBt_clicked",
    "",
    "on_mt_stopBt_clicked",
    "onReadSerialData",
    "processdata",
    "data",
    "on_debug_bt_clicked",
    "onDataReceivedFromB",
    "on_enseriBt_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSserialwindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    0,   58,    2, 0x08,    3 /* Private */,
       5,    1,   59,    2, 0x08,    4 /* Private */,
       7,    0,   62,    2, 0x08,    6 /* Private */,
       8,    1,   63,    2, 0x08,    7 /* Private */,
       9,    0,   66,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
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
        // method 'on_mc_startBt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_mt_stopBt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onReadSerialData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'processdata'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>,
        // method 'on_debug_bt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDataReceivedFromB'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_enseriBt_clicked'
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
        case 0: _t->on_mc_startBt_clicked(); break;
        case 1: _t->on_mt_stopBt_clicked(); break;
        case 2: _t->onReadSerialData(); break;
        case 3: _t->processdata((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->on_debug_bt_clicked(); break;
        case 5: _t->onDataReceivedFromB((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->on_enseriBt_clicked(); break;
        default: ;
        }
    }
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
