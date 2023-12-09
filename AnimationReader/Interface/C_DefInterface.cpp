#include "C_DefInterface.h"
#include "Widget/DefsTableUtils.h"
#include <QPushButton>
#include "winsock.h"
#pragma once

void CDefInterface::AddValueToTable(QTableWidget *tableWidget, QString header, QVariant value)
{
    int rowTotal = tableWidget->rowCount() + 1;
    tableWidget->setRowCount(rowTotal);
    header = header.contains("\"") ? header : "\"" + header + "\" :";
    tableWidget->setItem(rowTotal - 1, 0, new QTableWidgetItem(header));

    SyncedTableWidgetItem* tableItem = new SyncedTableWidgetItem();
    tableItem->setData(Qt::UserRole, 0x50); // default type
    bool opsRes = TypeHandler::HandleValue(value,tableItem); // Deals with types
    if (!opsRes){ qDebug() << " Could not handle for : " << value.typeName(); }

    tableItem->setText("");
    tableWidget->setItem(rowTotal - 1, 1, tableItem);
}

void CDefInterface::UI_Table_BuildEVNT(QTableWidget* table, StateNode::EventNode* event){

    if (event->name == ""){
        AddValueToTable(table, "Event GUID", QVariant::fromValue(&event->value_0));
    }
    else{
        AddValueToTable(table, "Event Name", QVariant::fromValue(&event->name));
    }

    AddValueToTable(table, "Event Paremeter", QVariant::fromValue(&event->value_1));
    /* Get Event Main Argument */
//    DefArg* arguement = &event->arguments[event->value_1].definition;
//    QString headerStr = "Event Parameter";

//    switch (arguement->type) {
//        case 0x5:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->value) );
//            break;
//        case 0x4:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->name) );
//            break;
//        case 0x3:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->fValue) );
//            break;
//        case 0x2:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->value) );
//            break;
//        case 0x1:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->flag) );
//            break;
//        default:
//            CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->value) );
//            break;
//    }

    BuildEvent(table,event);
}

void CDefInterface::UI_Table_BuildMEMB(QTableWidget* table, StateNode::MemberNode* member){
    switch (ntohl(member->type)) {
        case(0xEB7AD409):
            AddValueToTable(table, "value_0", QVariant::fromValue(&member->dValue_0) );
            break;
        case(0x76A50FDC):
            AddValueToTable(table, "value_0", QVariant::fromValue(&member->lValue_0) );
            AddValueToTable(table, "value_1", QVariant::fromValue(&member->encodeFlag) );
            break;
        case(0x4BFF8647):
            AddValueToTable(table, "value_0", QVariant::fromValue(&member->encodeFlag) );
            break;
        case(0x2F05210F):
            AddValueToTable(table, "value_0", QVariant::fromValue(&member->lValue_0) );
            AddValueToTable(table, "value_1", QVariant::fromValue(&member->encodeFlag) );
            break;
        case(0x1A8AECAF):
            AddValueToTable(table, "value_0", QVariant::fromValue(&member->lValue_0) );
            AddValueToTable(table, "value_1", QVariant::fromValue(&member->encodeFlag) );
            break;
        default:
            break;
    }
}

void CDefInterface::UI_Table_BuildCAND(QTableWidget* table, StateNode::Candidate* candidate){
    AddValueToTable(table, "value" , QVariant::fromValue(&candidate->value));
    AddValueToTable(table,"value", QVariant::fromValue(&candidate->flag));

    switch (ntohl(candidate->type)) {
        case(0xEB7AD409): //float, float
            AddValueToTable(table, "value_0", QVariant::fromValue(&candidate->fValue_0) );
            AddValueToTable(table, "value_1", QVariant::fromValue(&candidate->fValue_1) );
            break;
        case(0x4BFF8647): //float value
            AddValueToTable(table, "value_0", QVariant::fromValue(&candidate->fValue_0) );
            break;
        case(0x2F05210F): // short, float ,float
            AddValueToTable(table, "value_0", QVariant::fromValue(&candidate->sValue_0) );
            AddValueToTable(table, "value_1", QVariant::fromValue(&candidate->fValue_0) );
            AddValueToTable(table, "value_2", QVariant::fromValue(&candidate->fValue_1) );
            break;
        case(0x1A8AECAF): // short
            AddValueToTable(table, "value_0", QVariant::fromValue(&candidate->sValue_0) );
            break;
        default:
            break;
    }
}

void CDefInterface::InitializeArguments(QTableWidget* table, StateNode::EventNode* event){
    for (int i = 0; i < event->arguments.size(); i++){
        QString headerStr =  "argument #" + QString::number(i);
        DefArg* arguement = &event->arguments[i].definition;

        switch (arguement->type) {
            case 0x5:
                CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->value) );
                break;
            case 0x4:
                CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->name) );
                break;
            case 0x3:
                CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->fValue) );
                break;
            case 0x2:
                CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->value) );
                break;
            case 0x1:
                CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&arguement->flag) );
                break;
            default:
                break;
        }

    }
}

void CDefInterface::InitializeTriggers(QTableWidget* table, StateNode::EventNode* event){
    for (int i = 0; i < event->triggers.size(); i++){
        QString headerStr =  "trigger" + QString::number(i);
        CDefInterface::AddValueToTable(table, headerStr, QVariant::fromValue(&event->triggers[i].hash) );
        CDefInterface::AddValueToTable(table, headerStr+" value", QVariant::fromValue(&event->triggers[i].value) );
    }
}

void CDefInterface::BuildEvent(QTableWidget* table, StateNode::EventNode* event){
    if (!event){return;}
    if (event->flag){
        CDefInterface::AddValueToTable(table, "value_2", QVariant::fromValue(&event->value_2) );
        CDefInterface::AddValueToTable(table, "value_3", QVariant::fromValue(&event->value_3) ); }
    InitializeTriggers(table,event);
    InitializeArguments(table,event);
}

void CDefInterface::InitializeDESC(StateNode::Node* node, QTreeWidgetItem *root){
    for (std::string &descriptor :node->descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(root);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(&descriptor);    }
}

void CDefInterface::InitializeDESC(StateNode::GroupNode* node, QTreeWidgetItem *root){
    for (std::string &descriptor :node->descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(root);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(&descriptor);    }
}

void CDefInterface::InitializeMember(StateNode::GroupNode* group, DefsTreeWidgetItem *root){
    DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(root);
    membItem->setText(0, "MEMB Item");
    membItem->setData(0,Qt::UserRole+1, MEMB );
    membItem->setMemberNode(&group->members);
    for (auto &frame :group->members.frames ){
        DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
        frmsItem->setText(0, "FRMS Item");
        frmsItem->setData(0,Qt::UserRole+1, FRMS );
        frmsItem->setFrame(&frame);
    }
    for (auto &candidate :group->members.candidates ){
        DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
        candItem->setText(0, "CAND Item");
        candItem->setData(0,Qt::UserRole+1, CAND );
        candItem->setCandidate(&candidate);
    }
}

void CDefInterface::InitializeSelector(StateNode::GroupNode* group, DefsTreeWidgetItem *root){
    DefsTreeWidgetItem *selsItem = new DefsTreeWidgetItem(root);
    selsItem->setText(0, "SELS Item");
    selsItem->setData(0,Qt::UserRole+1, SELS );
    for (auto &member :group->selectors.members )
    {
        DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(selsItem);
        membItem->setText(0, "MEMB Item");
        membItem->setData(0,Qt::UserRole+1, MEMB );
        membItem->setMemberNode(&member);
        for (auto &frame :group->members.frames ){
            DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
            frmsItem->setText(0, "FRMS Item");
            frmsItem->setData(0,Qt::UserRole+1, FRMS );
            frmsItem->setFrame(&frame);  }
        for (auto &candidate :group->members.candidates ){
            DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
            candItem->setText(0, "CAND Item");
            candItem->setData(0,Qt::UserRole+1, CAND );
            candItem->setCandidate(&candidate);  }
    }
}

void CDefInterface::InitializeNode(StateNode::Node* node, QTreeWidgetItem *root){
    for (auto &key : node->keyValueProperties )
        if (ntohl(key.streamKey) == 0xBE83F7FD || ntohl(key.streamKey) == 0xDF4B0B8E)
            root->setText(0,key.chars.c_str());
    for (auto &childNode : node->childNodes )
        CDefInterface::UI_ConstructStateNode(&childNode,root);
}

void CDefInterface::InitializeSync(StateNode::Node* node, QTreeWidgetItem *root){
    for (auto &syncNode :node->syncNodes ){
        DefsTreeWidgetItem *syncItem = new DefsTreeWidgetItem(root);
        syncItem->setText(0, "SYNC Item");
        syncItem->setSyncNode(&syncNode);
        syncItem->setData(0,Qt::UserRole+1, SYNC );    }
}

void CDefInterface::InitializeDTT(StateNode::Node* node, QTreeWidgetItem *root){
    for (auto &dttNode :node->transNodes ){
        DefsTreeWidgetItem *dttItem = new DefsTreeWidgetItem(root);
        dttItem->setText(0, "DTT Item");
        dttItem->setStateNode(&dttNode);
        dttItem->setData(0,Qt::UserRole+1, DTT_ );  }
}

void CDefInterface::InitializeTOVR(StateNode::Node* node, QTreeWidgetItem *root){
    for (auto &ovlyNode :node->ovlyNodes ){
        DefsTreeWidgetItem *ovlyItem = new DefsTreeWidgetItem(root);
        ovlyItem->setText(0, "TOVR Item");
        ovlyItem->setStateNode(&ovlyNode);
        ovlyItem->setData(0,Qt::UserRole+1, TOVR );   }
}


void CDefInterface::InitializeEvent(StateNode::Node* node, QTreeWidgetItem *root){
    for ( int i = 0; i < node->events.size(); i++ ){
        DefsTreeWidgetItem *evntItem = new DefsTreeWidgetItem(root);
        evntItem->setText(0, "Event #" + QString::number(i) );
        evntItem->setData(0,Qt::UserRole+1, EVNT );
        evntItem->setEventNode(&node->events[i]);
        evntItem->setHidden(true);}
}

void CDefInterface::UI_ConstructGroupNode(StateNode::GroupNode* group, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, "GROUP: " + QString(group->chars.c_str()) );
    item->setData(0,Qt::UserRole+1, GRP_ );
    item->setGroupNode(group);
    InitializeDESC(group,item);
    InitializeMember(group,item);
    InitializeSelector(group,item);
}


void CDefInterface::UI_ConstructStateNode(StateNode::Node* node, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, (node->nodeType == STAT) ? "State Entry" : "Node");
    item->setStateNode(node);
    item->setData(0,Qt::UserRole+1, (node->nodeType == STAT) ? STAT : NODE );
    InitializeNode(node,item);
    InitializeSync(node,item);
    InitializeDTT(node,item);
    InitializeTOVR(node,item);
    InitializeDESC(node,item);
    InitializeEvent(node,item);
}

void CDefInterface::UI_ConstructDefsTree(QTreeWidget* tree, QTableWidget*table, std::vector<StateNode::Definition>* defs)
{
    uint32_t totalDefs = defs->size();
    tree->clear();
    table->clear();
    tree->setUpdatesEnabled(false);
    for (int i = 0; i < totalDefs; i++)
       UI_ConstructNewDefinition(tree,&defs->at(i),i);
    tree->setUpdatesEnabled(true);
}


void CDefInterface::UI_ConstructNewDefinition(QTreeWidget* tree, StateNode::Definition* animDef, int defIndex)
{
    DefsTreeWidgetItem *rootParent = new DefsTreeWidgetItem(tree);
    //Construct State sets
    for (auto &stateNode : animDef->stateNodes )
        CDefInterface::UI_ConstructStateNode(&stateNode,rootParent);
    //Construct Group sets
    for (auto &groupNode : animDef->groupNodes )
        CDefInterface::UI_ConstructGroupNode(&groupNode,rootParent);
    rootParent->setDefinition(animDef);
    rootParent->setText(0, "Definition #" + QString::number(defIndex) );
}


void CDefInterface::UI_Table_BuildSYNC(QTableWidget* table, DefsTreeWidgetItem* item){
    CDefInterface::AddValueToTable(table, "flag", QVariant::fromValue(&item->getSyncNode()->flag));
    CDefInterface::AddValueToTable(table, "value_0",QVariant::fromValue(&item->getSyncNode()->valueA));
    CDefInterface::AddValueToTable(table, "value_1", QVariant::fromValue(&item->getSyncNode()->valueB));
}

void CDefInterface::UI_Table_BuildDESC(QTableWidget* table, DefsTreeWidgetItem* item){
    for ( std::string* descriptor : item->getDescriptors() )
         CDefInterface::AddValueToTable(table, "desc", QVariant::fromValue(descriptor));
}
void CDefInterface::UI_Table_BuildFRMS(QTableWidget* table, DefsTreeWidgetItem* item){
    CDefInterface::AddValueToTable(table, "value_0", QVariant::fromValue(&item->getFrame()->value ));
    CDefInterface::AddValueToTable(table, "value_1", QVariant::fromValue(&item->getFrame()->flag));
}

void CDefInterface::UI_Table_BuildNODE(QTableWidget* table, DefsTreeWidgetItem* item){
    StateNode::Node* node =  item->getStateNode();
    for ( auto& prop : node->keyValueProperties){
        QString key = DefsTableUtils::FormatKeyValueString(prop);
        CDefInterface::AddValueToTable(table, key, DefsTableUtils::GetItemVariant(&prop) ); }
    if (node->streamType == 0x2){
        AddValueToTable(table,"int value",QVariant::fromValue(&node->value_0));
        AddValueToTable(table,"long value",QVariant::fromValue(&node->value_1));
        AddValueToTable(table,"float value",QVariant::fromValue(&node->floatVal)); }
    if (node->streamType == 0x1){ AddValueToTable(table,"flag",QVariant::fromValue(&node->flag)); }
    if (node->isBargNode){
        AddValueToTable(table,"long value",QVariant::fromValue(&node->value_1));
        AddValueToTable(table,"float value",QVariant::fromValue(&node->floatVal));  }
}

void CDefInterface::UI_Table_BuildKeyValue(QTableWidget* table, DefsTreeWidgetItem* item){
    StateNode::Node* node = item->getStateNode();
    if (!node){ return;}
    for ( auto& prop : node->keyValueProperties){
        QString key = DefsTableUtils::FormatKeyValueString(prop);
        CDefInterface::AddValueToTable(table, key, DefsTableUtils::GetItemVariant(&prop) ); }
}




