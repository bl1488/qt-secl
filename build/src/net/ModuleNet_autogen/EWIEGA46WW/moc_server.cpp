/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/net/server.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_net__Worker_t {
    uint offsetsAndSizes[38];
    char stringdata0[12];
    char stringdata1[6];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[3];
    char stringdata5[5];
    char stringdata6[10];
    char stringdata7[8];
    char stringdata8[7];
    char stringdata9[13];
    char stringdata10[22];
    char stringdata11[21];
    char stringdata12[16];
    char stringdata13[15];
    char stringdata14[5];
    char stringdata15[8];
    char stringdata16[12];
    char stringdata17[15];
    char stringdata18[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_net__Worker_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_net__Worker_t qt_meta_stringdata_net__Worker = {
    {
        QT_MOC_LITERAL(0, 11),  // "net::Worker"
        QT_MOC_LITERAL(12, 5),  // "Write"
        QT_MOC_LITERAL(18, 0),  // ""
        QT_MOC_LITERAL(19, 13),  // "std::uint64_t"
        QT_MOC_LITERAL(33, 2),  // "id"
        QT_MOC_LITERAL(36, 4),  // "data"
        QT_MOC_LITERAL(41, 9),  // "AddClient"
        QT_MOC_LITERAL(51, 7),  // "qintptr"
        QT_MOC_LITERAL(59, 6),  // "handle"
        QT_MOC_LITERAL(66, 12),  // "RemoveClient"
        QT_MOC_LITERAL(79, 21),  // "RequestClientInfoById"
        QT_MOC_LITERAL(101, 20),  // "RequestClientInfoAll"
        QT_MOC_LITERAL(122, 15),  // "ClientInfoReady"
        QT_MOC_LITERAL(138, 14),  // "ClientInfoData"
        QT_MOC_LITERAL(153, 4),  // "info"
        QT_MOC_LITERAL(158, 7),  // "DoWrite"
        QT_MOC_LITERAL(166, 11),  // "DoAddClient"
        QT_MOC_LITERAL(178, 14),  // "DoRemoveClient"
        QT_MOC_LITERAL(193, 19)   // "DoClientInfoRequest"
    },
    "net::Worker",
    "Write",
    "",
    "std::uint64_t",
    "id",
    "data",
    "AddClient",
    "qintptr",
    "handle",
    "RemoveClient",
    "RequestClientInfoById",
    "RequestClientInfoAll",
    "ClientInfoReady",
    "ClientInfoData",
    "info",
    "DoWrite",
    "DoAddClient",
    "DoRemoveClient",
    "DoClientInfoRequest"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_net__Worker[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   74,    2, 0x06,    1 /* Public */,
       6,    1,   79,    2, 0x06,    4 /* Public */,
       9,    1,   82,    2, 0x06,    6 /* Public */,
      10,    1,   85,    2, 0x06,    8 /* Public */,
      11,    0,   88,    2, 0x06,   10 /* Public */,
      12,    1,   89,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      15,    2,   92,    2, 0x08,   13 /* Private */,
      16,    1,   97,    2, 0x08,   16 /* Private */,
      17,    1,  100,    2, 0x08,   18 /* Private */,
      18,    1,  103,    2, 0x08,   20 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject net::Worker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_net__Worker.offsetsAndSizes,
    qt_meta_data_net__Worker,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_net__Worker_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Worker, std::true_type>,
        // method 'Write'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'AddClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qintptr, std::false_type>,
        // method 'RemoveClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>,
        // method 'RequestClientInfoById'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>,
        // method 'RequestClientInfoAll'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'ClientInfoReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ClientInfoData, std::false_type>,
        // method 'DoWrite'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'DoAddClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qintptr, std::false_type>,
        // method 'DoRemoveClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>,
        // method 'DoClientInfoRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<std::uint64_t, std::false_type>
    >,
    nullptr
} };

void net::Worker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Worker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Write((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 1: _t->AddClient((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1]))); break;
        case 2: _t->RemoveClient((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1]))); break;
        case 3: _t->RequestClientInfoById((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1]))); break;
        case 4: _t->RequestClientInfoAll(); break;
        case 5: _t->ClientInfoReady((*reinterpret_cast< std::add_pointer_t<ClientInfoData>>(_a[1]))); break;
        case 6: _t->DoWrite((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->DoAddClient((*reinterpret_cast< std::add_pointer_t<qintptr>>(_a[1]))); break;
        case 8: _t->DoRemoveClient((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1]))); break;
        case 9: _t->DoClientInfoRequest((*reinterpret_cast< std::add_pointer_t<std::uint64_t>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Worker::*)(std::uint64_t , const QString & );
            if (_t _q_method = &Worker::Write; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Worker::*)(qintptr );
            if (_t _q_method = &Worker::AddClient; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Worker::*)(std::uint64_t );
            if (_t _q_method = &Worker::RemoveClient; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Worker::*)(std::uint64_t );
            if (_t _q_method = &Worker::RequestClientInfoById; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Worker::*)();
            if (_t _q_method = &Worker::RequestClientInfoAll; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Worker::*)(ClientInfoData );
            if (_t _q_method = &Worker::ClientInfoReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *net::Worker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *net::Worker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_net__Worker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int net::Worker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void net::Worker::Write(std::uint64_t _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void net::Worker::AddClient(qintptr _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void net::Worker::RemoveClient(std::uint64_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void net::Worker::RequestClientInfoById(std::uint64_t _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void net::Worker::RequestClientInfoAll()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void net::Worker::ClientInfoReady(ClientInfoData _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
namespace {
struct qt_meta_stringdata_net__Server_t {
    uint offsetsAndSizes[12];
    char stringdata0[12];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[15];
    char stringdata4[5];
    char stringdata5[20];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_net__Server_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_net__Server_t qt_meta_stringdata_net__Server = {
    {
        QT_MOC_LITERAL(0, 11),  // "net::Server"
        QT_MOC_LITERAL(12, 22),  // "ServerInfoRequestReady"
        QT_MOC_LITERAL(35, 0),  // ""
        QT_MOC_LITERAL(36, 14),  // "ServerInfoData"
        QT_MOC_LITERAL(51, 4),  // "info"
        QT_MOC_LITERAL(56, 19)   // "OnServerInfoRequest"
    },
    "net::Server",
    "ServerInfoRequestReady",
    "",
    "ServerInfoData",
    "info",
    "OnServerInfoRequest"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_net__Server[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   29,    2, 0x0a,    3 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject net::Server::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_net__Server.offsetsAndSizes,
    qt_meta_data_net__Server,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_net__Server_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Server, std::true_type>,
        // method 'ServerInfoRequestReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const ServerInfoData &, std::false_type>,
        // method 'OnServerInfoRequest'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void net::Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Server *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->ServerInfoRequestReady((*reinterpret_cast< std::add_pointer_t<ServerInfoData>>(_a[1]))); break;
        case 1: _t->OnServerInfoRequest(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Server::*)(const ServerInfoData & );
            if (_t _q_method = &Server::ServerInfoRequestReady; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *net::Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *net::Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_net__Server.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int net::Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void net::Server::ServerInfoRequestReady(const ServerInfoData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
