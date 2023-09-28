#include "C_TableBehavior.h"
#include <sstream>


void UpdateUint64Ptr(SyncedTableWidgetItem* tableItem){
    bool ok;
    uint64_t* num = qvariant_cast<uint64_t*>(tableItem->getNodeProperty());
    uint64_t number = tableItem->text().toULongLong(&ok, 16);
    if (ok){ *num  = number; }
}

void UpdateBoolPtr(SyncedTableWidgetItem* tableItem){
    bool* num = qvariant_cast<bool*>(tableItem->getNodeProperty());
    if (tableItem->text() != "true" && tableItem->text() != "false"){ return; }
    *num = (tableItem->text() == "true") ? true : false;
}

void UpdateFloatPtr(SyncedTableWidgetItem* tableItem){
    bool ok;
    float* num = qvariant_cast<float*>(tableItem->getNodeProperty());
    float number = tableItem->text().toFloat(&ok);
    if (ok){ *num  = number; }
}

void UpdateUint32Ptr(SyncedTableWidgetItem* tableItem){
    bool ok;
    uint32_t* num = qvariant_cast<uint32_t*>(tableItem->getNodeProperty());
    uint32_t number = tableItem->text().toUInt(&ok);
    if (ok){ *num  = number; }
}

void UpdateUint16Ptr(SyncedTableWidgetItem* tableItem){
    bool ok;
    uint16_t* num = qvariant_cast<uint16_t*>(tableItem->getNodeProperty());
    uint16_t number = tableItem->text().toUInt(&ok);
    if (ok){ *num  = number; }
}

void UpdateInt32Ptr(SyncedTableWidgetItem* tableItem){
    bool ok;
    int32_t* num = qvariant_cast<int32_t*>(tableItem->getNodeProperty());
    int32_t number = tableItem->text().toUInt(&ok);
    if (ok){ *num  = number; }
}

void UpdateQStringPtr(SyncedTableWidgetItem* tableItem){
    QString* num = qvariant_cast<QString*>(tableItem->getNodeProperty());
    *num = tableItem->text();
}

void CTableBehavior::UpdateStdStringPtr(SyncedTableWidgetItem* tableItem){
    std::string* t = qvariant_cast<std::string*>(tableItem->getNodeProperty());
    *t = tableItem->text().toStdString();
}

void UpdateStdString(SyncedTableWidgetItem* tableItem){
    std::string t = qvariant_cast<std::string>(tableItem->getNodeProperty());
    t = tableItem->text().toStdString();
}
void CTableBehavior::UpdateTableWithNode(DefsTreeWidgetItem* item,
                                         QTableWidgetItem* tableItem, uint32_t type, QString header)
{
    SyncedTableWidgetItem *syncItem = dynamic_cast<SyncedTableWidgetItem*>(tableItem);
    if (!tableItem){ return; }
    switch (tableItem->data(Qt::UserRole).toUInt())
    {
        case(0x1):
            UpdateUint64Ptr(syncItem);
            break;
        case(0x2):
            UpdateBoolPtr(syncItem);
            break;
        case(0x3):
            UpdateFloatPtr(syncItem);
            break;
        case(0x4):
            UpdateUint32Ptr(syncItem);
            break;
        case(0x5):
            UpdateUint16Ptr(syncItem);
            break;
        case(0x6):
            UpdateInt32Ptr(syncItem);
            break;
        case(0x7):
            UpdateQStringPtr(syncItem);
            break;
        case(0x8):
            UpdateStdStringPtr(syncItem);
            break;
        case(0x9):
            UpdateStdString(syncItem);
            break;
        default:
            break;
    }
}

