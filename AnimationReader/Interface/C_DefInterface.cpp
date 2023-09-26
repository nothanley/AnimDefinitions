#include "C_DefInterface.h"
#include "DefsTableUtils.h"
#pragma once


void CDefInterface::UI_Table_BuildEVNT(QTableWidget* table, StateNode::EventNode event){
    AddValueToTable(table, "value_0", event.value_0 );
    AddValueToTable(table, "value_1", event.value_1 );
    BuildEvent(table,event);
}

void CDefInterface::UI_Table_BuildMEMB(QTableWidget* table, StateNode::MemberNode member){
    switch (ntohl(member.type)) {
        case(0xEB7AD409):
            AddValueToTable(table, "value_0", member.dValue_0 );
            break;
        case(0x76A50FDC):
            AddValueToTable(table, "value_0", member.lValue_0 );
            AddValueToTable(table, "value_1", member.encodeFlag );
            break;
        case(0x4BFF8647):
            AddValueToTable(table, "value_0", member.encodeFlag );
            break;
        case(0x2F05210F):
            AddValueToTable(table, "value_0", member.lValue_0 );
            AddValueToTable(table, "value_1", member.encodeFlag );
            break;
        case(0x1A8AECAF):
            AddValueToTable(table, "value_0", member.lValue_0 );
            AddValueToTable(table, "value_1", member.encodeFlag );
            break;
        default:
            break;
    }
}

void CDefInterface::UI_Table_BuildCAND(QTableWidget* table, StateNode::Candidate candidate){
    AddValueToTable(table, "value" , candidate.value);
    AddValueToTable(table,"value",candidate.flag);

    switch (ntohl(candidate.type)) {
        case(0xEB7AD409): //float, float
            AddValueToTable(table, "value_0", candidate.fValue_0 );
            AddValueToTable(table, "value_1", candidate.fValue_1 );
            break;
        case(0x4BFF8647): //float value
            AddValueToTable(table, "value_0", candidate.fValue_0 );
            break;
        case(0x2F05210F): // short, float ,float
            AddValueToTable(table, "value_0", candidate.sValue_0 );
            AddValueToTable(table, "value_1", candidate.fValue_0 );
            AddValueToTable(table, "value_2", candidate.fValue_1 );
            break;
        case(0x1A8AECAF): // short
            AddValueToTable(table, "value_0", candidate.sValue_0 );
            break;
        default:
            break;
    }
}

void CDefInterface::InitializeArguments(QTableWidget* table, StateNode::EventNode event){
    for (int i = 0; i < event.arguments.size(); i++){
        QString headerStr =  "arg_" + QString::number(i);
        CDefInterface::AddValueToTable(table, headerStr, event.arguments[i].index );
    }
}

void CDefInterface::InitializeTriggers(QTableWidget* table, StateNode::EventNode event){
    for (int i = 0; i < event.triggers.size(); i++){
        QString headerStr =  "trig_" + QString::number(i);
        CDefInterface::AddValueToTable(table, headerStr, event.triggers[i].hash );
        CDefInterface::AddValueToTable(table, headerStr+" value", event.triggers[i].value );
    }
}

void CDefInterface::BuildEvent(QTableWidget* table, StateNode::EventNode event){
    if (event.flag){
        CDefInterface::AddValueToTable(table, "value_2", event.value_2 );
        CDefInterface::AddValueToTable(table, "value_3", event.value_3 ); }
    InitializeTriggers(table,event);
    InitializeArguments(table,event);
}

void CDefInterface::InitializeDESC(StateNode::Node node, QTreeWidgetItem *root){
    for (const auto &descriptor :node.descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(root);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(descriptor);    }
}

void CDefInterface::InitializeDESC(StateNode::GroupNode node, QTreeWidgetItem *root){
    for (const auto &descriptor :node.descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(root);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(descriptor);    }
}

void CDefInterface::InitializeMember(StateNode::GroupNode group, DefsTreeWidgetItem *root){
    DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(root);
    membItem->setText(0, "MEMB Item");
    membItem->setData(0,Qt::UserRole+1, MEMB );
    membItem->setMemberNode(group.members);
    for (const auto &frame :group.members.frames ){
        DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
        frmsItem->setText(0, "FRMS Item");
        frmsItem->setData(0,Qt::UserRole+1, FRMS );
        frmsItem->setFrame(frame);
    }
    for (const auto &candidate :group.members.candidates ){
        DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
        candItem->setText(0, "CAND Item");
        candItem->setData(0,Qt::UserRole+1, CAND );
        candItem->setCandidate(candidate);
    }
}

void CDefInterface::InitializeSelector(StateNode::GroupNode group, DefsTreeWidgetItem *root){
    DefsTreeWidgetItem *selsItem = new DefsTreeWidgetItem(root);
    selsItem->setText(0, "SELS Item");
    selsItem->setData(0,Qt::UserRole+1, SELS );
    for (const auto &member :group.selectors.members )
    {
        DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(selsItem);
        membItem->setText(0, "MEMB Item");
        membItem->setData(0,Qt::UserRole+1, MEMB );
        membItem->setMemberNode(member);
        for (const auto &frame :group.members.frames ){
            DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
            frmsItem->setText(0, "FRMS Item");
            frmsItem->setData(0,Qt::UserRole+1, FRMS );
            frmsItem->setFrame(frame);  }
        for (const auto &candidate :group.members.candidates ){
            DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
            candItem->setText(0, "CAND Item");
            candItem->setData(0,Qt::UserRole+1, CAND );
            candItem->setCandidate(candidate);  }
    }
}

void CDefInterface::InitializeNode(StateNode::Node node, QTreeWidgetItem *root){
    for (const auto &key:node.keyValueProperties )
        if (ntohl(key.streamKey) == 0xBE83F7FD)
            root->setText(0,key.chars.c_str());
    for (const auto &childNode:node.childNodes )
        CDefInterface::UI_ConstructStateNode(childNode,root);
}

void CDefInterface::InitializeSync(StateNode::Node node, QTreeWidgetItem *root){
    for (const auto &syncNode :node.syncNodes ){
        DefsTreeWidgetItem *syncItem = new DefsTreeWidgetItem(root);
        syncItem->setText(0, "SYNC Item");
        syncItem->setSyncNode(syncNode);
        syncItem->setData(0,Qt::UserRole+1, SYNC );    }
}

void CDefInterface::InitializeDTT(StateNode::Node node, QTreeWidgetItem *root){
    for (const auto &dttNode :node.transNodes ){
        DefsTreeWidgetItem *dttItem = new DefsTreeWidgetItem(root);
        dttItem->setText(0, "DTT_ Item");
        dttItem->setStateNode(dttNode);
        dttItem->setData(0,Qt::UserRole+1, DTT_ );  }
}

void CDefInterface::InitializeTOVR(StateNode::Node node, QTreeWidgetItem *root){
    for (const auto &ovlyNode :node.ovlyNodes ){
        DefsTreeWidgetItem *ovlyItem = new DefsTreeWidgetItem(root);
        ovlyItem->setText(0, "TOVR Item");
        ovlyItem->setStateNode(ovlyNode);
        ovlyItem->setData(0,Qt::UserRole+1, TOVR );   }
}


void CDefInterface::InitializeEvent(StateNode::Node node, QTreeWidgetItem *root){
    for ( int i = 0; i < node.events.size(); i++ ){
        DefsTreeWidgetItem *evntItem = new DefsTreeWidgetItem(root);
        evntItem->setText(0, "Event #" + QString::number(i) );
        evntItem->setData(0,Qt::UserRole+1, EVNT );
        evntItem->setEventNode(node.events[i]);}
}

void CDefInterface::UI_ConstructGroupNode(StateNode::GroupNode group, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, "GROUP: " + QString(group.chars.c_str()) );
    item->setData(0,Qt::UserRole+1, GRP_ );
    item->setGroupNode(group);
    InitializeDESC(group,item);
    InitializeMember(group,item);
    InitializeSelector(group,item);
}


void CDefInterface::UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, (node.nodeType == STAT) ? "State Entry" : "Node");
    item->setStateNode(node);
    item->setData(0,Qt::UserRole+1, (node.nodeType == STAT) ? STAT : NODE );
    InitializeNode(node,item);
    InitializeSync(node,item);
    InitializeDTT(node,item);
    InitializeTOVR(node,item);
    InitializeDESC(node,item);
    InitializeEvent(node,item);;
}

void CDefInterface::UI_ConstructDefsTree(QTreeWidget* tree, QTableWidget*table, std::vector<StateNode::Definition> defs)
{
    uint32_t totalDefs = defs.size();
    tree->clear();
    table->clear();
    for (int i = 0; i < totalDefs; i++)
       UI_ConstructNewDefinition(tree,defs[i],i);
}


void CDefInterface::UI_ConstructNewDefinition(QTreeWidget* tree, StateNode::Definition animDef, int defIndex)
{
    DefsTreeWidgetItem *rootParent = new DefsTreeWidgetItem(tree);
    //Construct State sets
    for (const auto &stateNode:animDef.stateNodes )
        CDefInterface::UI_ConstructStateNode(stateNode,rootParent);
    //Construct Group sets
    for (const auto &groupNode:animDef.groupNodes )
        CDefInterface::UI_ConstructGroupNode(groupNode,rootParent);
    rootParent->setText(0, "Definition #" + QString::number(defIndex) );
}


void CDefInterface::UI_Table_BuildSYNC(QTableWidget* table, DefsTreeWidgetItem* item){
    CDefInterface::AddValueToTable(table, "flag", item->getSyncNode().flag);
    CDefInterface::AddValueToTable(table, "value_0", item->getSyncNode().valueA);
    CDefInterface::AddValueToTable(table, "value_1", item->getSyncNode().valueB);
}

void CDefInterface::UI_Table_BuildDESC(QTableWidget* table, DefsTreeWidgetItem* item){
    for (const auto& descriptor : item->getDescriptors() )
         CDefInterface::AddValueToTable(table, "desc", QString(descriptor.c_str()) );
}
void CDefInterface::UI_Table_BuildFRMS(QTableWidget* table, DefsTreeWidgetItem* item){
    CDefInterface::AddValueToTable(table, "value_0", item->getFrame().value );
    CDefInterface::AddValueToTable(table, "value_1", item->getFrame().flag);
}

void CDefInterface::UI_Table_BuildNODE(QTableWidget* table, DefsTreeWidgetItem* item){
    StateNode::Node node =  item->getStateNode();
    for (const auto& prop : node.keyValueProperties){
        QString key = FormatKeyValueString(prop);
        QString value = FormatKeyValueString(prop,true);
        CDefInterface::AddValueToTable(table, key, value ); }

    if (node.streamType == 0x2){
        AddValueToTable(table,"value",node.value_0);
        AddValueToTable(table,"value",node.value_1);
        AddValueToTable(table,"value",node.floatVal); }
    if (node.streamType == 0x1){ AddValueToTable(table,"flag",node.flag); }
    if (node.isBargNode){
        AddValueToTable(table,"value",node.value_1);
        AddValueToTable(table,"value",node.floatVal);  }
}

void CDefInterface::UI_Table_BuildKeyValue(QTableWidget* table, DefsTreeWidgetItem* item){
    for (const auto& prop : item->getStateNode().keyValueProperties){
        QString key = FormatKeyValueString(prop);
        QString value = FormatKeyValueString(prop,true);
        CDefInterface::AddValueToTable(table, key, value ); }
}




