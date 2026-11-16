/****************************************************************************
** Meta object code from reading C++ file 'foodcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../foodcontroller.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'foodcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN14FoodControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto FoodController::qt_create_metaobjectdata<qt_meta_tag_ZN14FoodControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FoodController",
        "dataChanged",
        "",
        "messageChanged",
        "message",
        "generateDay",
        "replaceMeal",
        "mealType",
        "addToDayMenu",
        "dishId",
        "addProduct",
        "product",
        "removeProduct",
        "index",
        "addDish",
        "name",
        "recipe",
        "calories",
        "protein",
        "fat",
        "carbs",
        "ingredients",
        "imagePath",
        "recipeById",
        "QVariantMap",
        "id",
        "today",
        "QVariantList",
        "allRecipes",
        "myRecipes",
        "products",
        "recipeCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'dataChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'messageChanged'
        QtMocHelpers::SignalData<void(const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Method 'generateDay'
        QtMocHelpers::MethodData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'replaceMeal'
        QtMocHelpers::MethodData<void(int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Method 'addToDayMenu'
        QtMocHelpers::MethodData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Method 'addProduct'
        QtMocHelpers::MethodData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Method 'removeProduct'
        QtMocHelpers::MethodData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Method 'addDish'
        QtMocHelpers::MethodData<bool(const QString &, int, const QString &, double, double, double, double, const QString &, const QString &)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 15 }, { QMetaType::Int, 7 }, { QMetaType::QString, 16 }, { QMetaType::Double, 17 },
            { QMetaType::Double, 18 }, { QMetaType::Double, 19 }, { QMetaType::Double, 20 }, { QMetaType::QString, 21 },
            { QMetaType::QString, 22 },
        }}),
        // Method 'recipeById'
        QtMocHelpers::MethodData<QVariantMap(int) const>(23, 2, QMC::AccessPublic, 0x80000000 | 24, {{
            { QMetaType::Int, 25 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'today'
        QtMocHelpers::PropertyData<QVariantList>(26, 0x80000000 | 27, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'allRecipes'
        QtMocHelpers::PropertyData<QVariantList>(28, 0x80000000 | 27, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'myRecipes'
        QtMocHelpers::PropertyData<QVariantList>(29, 0x80000000 | 27, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'products'
        QtMocHelpers::PropertyData<QStringList>(30, QMetaType::QStringList, QMC::DefaultPropertyFlags, 0),
        // property 'recipeCount'
        QtMocHelpers::PropertyData<int>(31, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FoodController, qt_meta_tag_ZN14FoodControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FoodController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FoodControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FoodControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14FoodControllerE_t>.metaTypes,
    nullptr
} };

void FoodController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FoodController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dataChanged(); break;
        case 1: _t->messageChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->generateDay(); break;
        case 3: _t->replaceMeal((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->addToDayMenu((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->addProduct((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->removeProduct((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: { bool _r = _t->addDish((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[8])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[9])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: { QVariantMap _r = _t->recipeById((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantMap*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FoodController::*)()>(_a, &FoodController::dataChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FoodController::*)(const QString & )>(_a, &FoodController::messageChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->today(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->allRecipes(); break;
        case 2: *reinterpret_cast<QVariantList*>(_v) = _t->myRecipes(); break;
        case 3: *reinterpret_cast<QStringList*>(_v) = _t->products(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->recipeCount(); break;
        default: break;
        }
    }
}

const QMetaObject *FoodController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FoodController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14FoodControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FoodController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FoodController::dataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FoodController::messageChanged(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
