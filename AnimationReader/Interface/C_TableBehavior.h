#ifndef C_TABLEBEHAVIOR_H
#define C_TABLEBEHAVIOR_H
#include "DefsTreeWidgetItem.h"
#include <QTableWidget>

class CTableBehavior
{
public:
    static void UpdateNode(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateState(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateMember(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateEvent(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateFrame(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateCandidate(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, QString header);
    static void UpdateTableWithNode(DefsTreeWidgetItem* item, QTableWidgetItem* tableItem, uint32_t type, QString header);
};

#endif // C_TABLEBEHAVIOR_H
