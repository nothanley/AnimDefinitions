#ifndef TYPEHANDLER_H
#define TYPEHANDLER_H

#include <QVariant>
#include <QMetaType>
#include <functional>
#include <unordered_map>
#include <QDebug>
#include "Interface/C_SyncedTableWidgetItem.h"
#pragma once

// Define custom metatype IDs
Q_DECLARE_METATYPE(uint64_t*)
Q_DECLARE_METATYPE(uint32_t*)
Q_DECLARE_METATYPE(uint16_t*)
Q_DECLARE_METATYPE(int32_t*)
Q_DECLARE_METATYPE(QString*)
Q_DECLARE_METATYPE(std::string *)
Q_DECLARE_METATYPE(bool*)
Q_DECLARE_METATYPE(float*)
Q_DECLARE_METATYPE(std::string)

class TypeHandler{
public:
// Handling functions for different types
static void HandleUInt64Ptr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x1);
    tableItem->setData(Qt::UserRole+21,value);
}
static void HandleBoolPtr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x2);
    tableItem->setData(Qt::UserRole+22,value);
}
static void HandleFloatPtr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x3);
    tableItem->setData(Qt::UserRole+23,value);
}
static void HandleUInt32Ptr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x4);
    tableItem->setData(Qt::UserRole+24,value);
}
static void HandleUInt16Ptr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x5);
    tableItem->setData(Qt::UserRole+25,value);
}
static void HandleInt32Ptr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x6);
    tableItem->setData(Qt::UserRole+26,value);
}
static void HandleQStringPtr(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x7);
    tableItem->setData(Qt::UserRole+27,value);
}
static void HandleConstString(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x8);
    tableItem->setData(Qt::UserRole+28,value);
}
static void HandleStdString(QVariant& value, SyncedTableWidgetItem* tableItem) {
    tableItem->setNodeProperty(value);
    tableItem->setData(Qt::UserRole, 0x9);
    tableItem->setData(Qt::UserRole+29,value);
}


static bool HandleValue(QVariant& value, SyncedTableWidgetItem* tableItem) {
    int UInt64PtrType = QMetaType::type("uint64_t*");
    int UInt32PtrType = QMetaType::type("uint32_t*");
    int UInt16PtrType = QMetaType::type("uint16_t*");
    int Int32PtrType = QMetaType::type("int32_t*");
    int QStringPtrType = QMetaType::type("QString*");
    int BoolPtrType = QMetaType::type("bool*");
    int FloatPtrType = QMetaType::type("float*");
    int StdStrPtrType = QMetaType::type("std::string *");
    int StdStrType = QMetaType::type("std::string");

    // Function map to associate metatype IDs with handling functions
    std::unordered_map<int, std::function<void(QVariant&,SyncedTableWidgetItem*)>> handlerMap = {
        { UInt64PtrType, TypeHandler::HandleUInt64Ptr },
        { UInt32PtrType, TypeHandler::HandleUInt32Ptr },
        { UInt16PtrType, TypeHandler::HandleUInt16Ptr },
        { Int32PtrType, TypeHandler::HandleInt32Ptr },
        { QStringPtrType, TypeHandler::HandleQStringPtr },
        { BoolPtrType, TypeHandler::HandleBoolPtr },
        { FloatPtrType, TypeHandler::HandleFloatPtr },
        { StdStrPtrType, TypeHandler::HandleConstString },
        { StdStrType, TypeHandler::HandleStdString }
    };
    auto it = handlerMap.find(value.userType());
    if (it != handlerMap.end()) {
        it->second(value,tableItem);
    }else { return false;  }

    return true;
}


};

#endif // TYPEHANDLER_H
