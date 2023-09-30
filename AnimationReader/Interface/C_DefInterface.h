
#include <QTableWidget>
#include "AnimDefinitions/AnimDefHandler.h"
#include "DefsTreeWidgetItem.h"
#include "TypeHandler.h"
#pragma once

class CDefInterface
{
private:

    static void InitializeArguments(QTableWidget* table, StateNode::EventNode* event);
    static void InitializeTriggers(QTableWidget* table, StateNode::EventNode* event);
    static void BuildEvent(QTableWidget* table, StateNode::EventNode* event);
    static void InitializeDESC(StateNode::Node* node, QTreeWidgetItem *root);
    static void InitializeDESC(StateNode::GroupNode* node, QTreeWidgetItem *root);
    static void InitializeMember(StateNode::GroupNode* group, DefsTreeWidgetItem *root);
    static void InitializeSelector(StateNode::GroupNode* group, DefsTreeWidgetItem *root);
    static void InitializeNode(StateNode::Node* node, QTreeWidgetItem *root);
    static void InitializeSync(StateNode::Node* node, QTreeWidgetItem *root);
    static void InitializeDTT(StateNode::Node* node, QTreeWidgetItem *root);
    static void InitializeTOVR(StateNode::Node* node, QTreeWidgetItem *root);
    static void InitializeEvent(StateNode::Node* node, QTreeWidgetItem *root);

public:
    static void UI_Table_BuildSYNC(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildDESC(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildFRMS(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildNODE(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildKeyValue(QTableWidget* table, DefsTreeWidgetItem* item);
    static void UI_Table_BuildEVNT(QTableWidget* table, StateNode::EventNode* event);
    static void UI_Table_BuildMEMB(QTableWidget* table, StateNode::MemberNode* member);
    static void UI_Table_BuildCAND(QTableWidget* table, StateNode::Candidate* candidate);
    static void UI_ConstructStateNode(Node *node, QTreeWidgetItem *parent);
    static void UI_ConstructGroupNode(GroupNode *node, QTreeWidgetItem *parent);
    static void UI_ConstructDefsTree(QTreeWidget* tree, QTableWidget*table, std::vector<Definition> *defs);
    static void UI_ConstructNewDefinition(QTreeWidget* tree,StateNode::Definition* animDef, int defIndex);

    static void AddValueToTable(QTableWidget* tableWidget, QString header, QVariant value);

};


