/****************************************************************************
** Meta object code from reading C++ file 'graph_area.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gui/graph_area.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graph_area.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_graph_area_t {
    QByteArrayData data[14];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_graph_area_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_graph_area_t qt_meta_stringdata_graph_area = {
    {
QT_MOC_LITERAL(0, 0, 10), // "graph_area"
QT_MOC_LITERAL(1, 11, 21), // "update_active_objects"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10), // "add_source"
QT_MOC_LITERAL(4, 45, 8), // "add_sink"
QT_MOC_LITERAL(5, 54, 8), // "add_pipe"
QT_MOC_LITERAL(6, 63, 15), // "add_link_pushed"
QT_MOC_LITERAL(7, 79, 8), // "add_link"
QT_MOC_LITERAL(8, 88, 7), // "vertex*"
QT_MOC_LITERAL(9, 96, 1), // "f"
QT_MOC_LITERAL(10, 98, 1), // "s"
QT_MOC_LITERAL(11, 100, 18), // "QGraphicsLineItem*"
QT_MOC_LITERAL(12, 119, 9), // "link_line"
QT_MOC_LITERAL(13, 129, 14) // "object_updated"

    },
    "graph_area\0update_active_objects\0\0"
    "add_source\0add_sink\0add_pipe\0"
    "add_link_pushed\0add_link\0vertex*\0f\0s\0"
    "QGraphicsLineItem*\0link_line\0"
    "object_updated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_graph_area[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    3,   54,    2, 0x0a /* Public */,
      13,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 8, 0x80000000 | 11,    9,   10,   12,
    QMetaType::Void,

       0        // eod
};

void graph_area::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<graph_area *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->update_active_objects(); break;
        case 1: _t->add_source(); break;
        case 2: _t->add_sink(); break;
        case 3: _t->add_pipe(); break;
        case 4: _t->add_link_pushed(); break;
        case 5: _t->add_link((*reinterpret_cast< vertex*(*)>(_a[1])),(*reinterpret_cast< vertex*(*)>(_a[2])),(*reinterpret_cast< QGraphicsLineItem*(*)>(_a[3]))); break;
        case 6: _t->object_updated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (graph_area::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&graph_area::update_active_objects)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject graph_area::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_graph_area.data,
    qt_meta_data_graph_area,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *graph_area::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *graph_area::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_graph_area.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int graph_area::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void graph_area::update_active_objects()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
