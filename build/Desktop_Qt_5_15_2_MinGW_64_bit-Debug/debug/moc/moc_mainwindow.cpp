/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../../../../code-beidou/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata0[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 13), // "onStartReplay"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "onStopReplay"
QT_MOC_LITERAL(4, 39, 13), // "onPauseReplay"
QT_MOC_LITERAL(5, 53, 14), // "onResumeReplay"
QT_MOC_LITERAL(6, 68, 14), // "onSpeedChanged"
QT_MOC_LITERAL(7, 83, 5), // "speed"
QT_MOC_LITERAL(8, 89, 17), // "onProgressChanged"
QT_MOC_LITERAL(9, 107, 5), // "value"
QT_MOC_LITERAL(10, 113, 13), // "onDataUpdated"
QT_MOC_LITERAL(11, 127, 13), // "SatelliteData"
QT_MOC_LITERAL(12, 141, 4), // "data"
QT_MOC_LITERAL(13, 146, 14), // "onShowNMEAView"
QT_MOC_LITERAL(14, 161, 15), // "onShowBasicView"
QT_MOC_LITERAL(15, 177, 17), // "onShowMessageView"
QT_MOC_LITERAL(16, 195, 19), // "onShowSatelliteView"
QT_MOC_LITERAL(17, 215, 13), // "onShowSNRView"
QT_MOC_LITERAL(18, 229, 14), // "onShowAllViews"
QT_MOC_LITERAL(19, 244, 14), // "onHideAllViews"
QT_MOC_LITERAL(20, 259, 14), // "onToggleLayout"
QT_MOC_LITERAL(21, 274, 10), // "closeEvent"
QT_MOC_LITERAL(22, 285, 12), // "QCloseEvent*"
QT_MOC_LITERAL(23, 298, 5) // "event"

    },
    "MainWindow\0onStartReplay\0\0onStopReplay\0"
    "onPauseReplay\0onResumeReplay\0"
    "onSpeedChanged\0speed\0onProgressChanged\0"
    "value\0onDataUpdated\0SatelliteData\0"
    "data\0onShowNMEAView\0onShowBasicView\0"
    "onShowMessageView\0onShowSatelliteView\0"
    "onShowSNRView\0onShowAllViews\0"
    "onHideAllViews\0onToggleLayout\0closeEvent\0"
    "QCloseEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    1,   98,    2, 0x08 /* Private */,
       8,    1,  101,    2, 0x08 /* Private */,
      10,    1,  104,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,
      18,    0,  112,    2, 0x08 /* Private */,
      19,    0,  113,    2, 0x08 /* Private */,
      20,    0,  114,    2, 0x08 /* Private */,
      21,    1,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   23,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onStartReplay(); break;
        case 1: _t->onStopReplay(); break;
        case 2: _t->onPauseReplay(); break;
        case 3: _t->onResumeReplay(); break;
        case 4: _t->onSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onProgressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onDataUpdated((*reinterpret_cast< const SatelliteData(*)>(_a[1]))); break;
        case 7: _t->onShowNMEAView(); break;
        case 8: _t->onShowBasicView(); break;
        case 9: _t->onShowMessageView(); break;
        case 10: _t->onShowSatelliteView(); break;
        case 11: _t->onShowSNRView(); break;
        case 12: _t->onShowAllViews(); break;
        case 13: _t->onHideAllViews(); break;
        case 14: _t->onToggleLayout(); break;
        case 15: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
