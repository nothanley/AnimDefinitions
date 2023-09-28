#ifndef C_TABLEBEHAVIOR_H
#define C_TABLEBEHAVIOR_H
#include "DefsTreeWidgetItem.h"
#include <QTableWidget>
#include "Interface/C_SyncedTableWidgetItem.h"

class CTableBehavior
{
public:
    static void UpdateStdStringPtr(SyncedTableWidgetItem *tableItem);
    static void UpdateTableWithNode(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, uint32_t type, QString header);
};

#endif // C_TABLEBEHAVIOR_H
