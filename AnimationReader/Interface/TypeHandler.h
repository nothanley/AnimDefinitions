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
    // Function map to associate metatype IDs with handling functions
    std::unordered_map<int, std::function<void(QVariant&,SyncedTableWidgetItem*)>> handlerMap = {
        { QMetaType::type("uint64_t*"), TypeHandler::HandleUInt64Ptr },
        { QMetaType::type("uint32_t*"), TypeHandler::HandleUInt32Ptr },
        { QMetaType::type("uint16_t*"), TypeHandler::HandleUInt16Ptr },
        { QMetaType::type("int32_t*"), TypeHandler::HandleInt32Ptr },
        { QMetaType::type("QString*"), TypeHandler::HandleQStringPtr },
        { QMetaType::type("bool*"), TypeHandler::HandleBoolPtr },
        { QMetaType::type("float*"), TypeHandler::HandleFloatPtr },
        { QMetaType::type("std::string *"), TypeHandler::HandleConstString },
        { QMetaType::type("std::string"), TypeHandler::HandleStdString }
    };
    auto it = handlerMap.find(value.userType());
    if (it != handlerMap.end()) {
        it->second(value,tableItem); }
    else { return false;  }
    return true;
}


};

#endif // TYPEHANDLER_H
