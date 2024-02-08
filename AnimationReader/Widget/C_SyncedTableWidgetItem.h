#ifndef C_SYNCEDTABLEWIDGETITEM_H
#define C_SYNCEDTABLEWIDGETITEM_H
#include <QTableWidgetItem>


class SyncedTableWidgetItem : public QTableWidgetItem
{
public:

    explicit SyncedTableWidgetItem(int type = Type) : QTableWidgetItem(type) {}
    explicit SyncedTableWidgetItem(const QString &text, int type = Type) :  QTableWidgetItem(text,type) {}
    SyncedTableWidgetItem(const QTableWidgetItem &other) : QTableWidgetItem(other){}
    void setNodeProperty(QVariant prop){ this->m_NodeProperty = prop;}
    QVariant getNodeProperty(){ return this->m_NodeProperty; }

private:
    QVariant m_NodeProperty;
};

#endif // C_SYNCEDTABLEWIDGETITEM_H
