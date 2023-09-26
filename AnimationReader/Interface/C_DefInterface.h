#include <QDebug>
#include <QTableWidget>
#include "AnimDefinitions/AnimDefHandler.h"
#include "DefsTreeWidgetItem.h"
#pragma once

class CDefInterface
{
private:
    static void InitializeArguments(QTableWidget* table, StateNode::EventNode event);
    static void InitializeTriggers(QTableWidget* table, StateNode::EventNode event);
    static void BuildEvent(QTableWidget* table, StateNode::EventNode event);
    static void InitializeDESC(StateNode::Node node, QTreeWidgetItem *root);
    static void InitializeDESC(StateNode::GroupNode node, QTreeWidgetItem *root);
    static void InitializeMember(StateNode::GroupNode group, DefsTreeWidgetItem *root);
    static void InitializeSelector(StateNode::GroupNode group, DefsTreeWidgetItem *root);
    static void InitializeNode(StateNode::Node node, QTreeWidgetItem *root);
    static void InitializeSync(StateNode::Node node, QTreeWidgetItem *root);
    static void InitializeDTT(StateNode::Node node, QTreeWidgetItem *root);
    static void InitializeTOVR(StateNode::Node node, QTreeWidgetItem *root);
    static void InitializeEvent(StateNode::Node node, QTreeWidgetItem *root);

public:
    static void UI_Table_BuildSYNC(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildDESC(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildFRMS(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildNODE(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildKeyValue(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildEVNT(QTableWidget* table, StateNode::EventNode event);
    static void UI_Table_BuildMEMB(QTableWidget* table, StateNode::MemberNode member);
    static void UI_Table_BuildCAND(QTableWidget* table, StateNode::Candidate candidate);
    static void UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent);
    static void UI_ConstructGroupNode(StateNode::GroupNode node, QTreeWidgetItem *parent);
    static void UI_ConstructDefsTree(QTreeWidget* tree, QTableWidget*table, std::vector<StateNode::Definition> defs);
    static void UI_ConstructNewDefinition(QTreeWidget* tree,StateNode::Definition animDef, int defIndex);

    template <typename T>
    static void AddValueToTable(QTableWidget* tableWidget, QString header, T value)
    {
        int rowTotal = tableWidget->rowCount() + 1;
        tableWidget->setRowCount(rowTotal);
        header = header.contains("\"") ? header : "\"" + header + "\" :";
        tableWidget->setItem(rowTotal - 1, 0, new QTableWidgetItem(header));
        QString itemText;

        if constexpr (std::is_same<T, QString>::value) {
            itemText = value;
        } else if constexpr (std::is_same<T, uint64_t>::value) {
            itemText = "\"" + QString::number(value, 16) + "\"";
        } else if constexpr (std::is_same<T, bool>::value) {
            itemText = value ? "true" : "false";
        } else if constexpr (std::is_same<T, float>::value) {
            itemText = QString::number(value);
        } else if constexpr (std::is_same<T, uint32_t>::value) {
            itemText = QString::number(value);
        } else if constexpr (std::is_same<T, uint16_t>::value) {
            itemText = QString::number(value);
        }
        else{ qDebug() << "[DEBUG] No var type"; return; }

        tableWidget->setItem(rowTotal - 1, 1, new QTableWidgetItem(itemText));
    }

};


