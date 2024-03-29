/* Subclass for custom TreeWidgetItem's enabling storing additional required data for animation definitions */
#ifndef DEFSTREEWIDGETITEM_H
#define DEFSTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "ColorItemDelegate.h"
#include "DefsReader.h"
#include <QMessageBox>
#pragma once

class DefsTreeWidgetItem : public QTreeWidgetItem
{
public:
    virtual void item() const {}
    DefsTreeWidgetItem(QTreeWidget *treeview, int type = Type)  : QTreeWidgetItem(treeview,type) {};
    DefsTreeWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent,type) {};

    void setStateNode(StateNode::Node* node) { m_DefsNode = node; }
    StateNode::Node* getStateNode(){return this->m_DefsNode;}

    void setSyncNode(StateNode::SyncNode* node) { m_SyncNode = node; }
    StateNode::SyncNode* getSyncNode(){return this->m_SyncNode;}

    void setGroupNode(StateNode::GroupNode* node) { m_GroupNode = node; }
    StateNode::GroupNode* getGroupNode(){return this->m_GroupNode;}

    void setEventNode(StateNode::EventNode* node) { m_EventNode = node; }
    StateNode::EventNode* getEventNode(){return this->m_EventNode;}

    void setMemberNode(StateNode::MemberNode* node) { m_MemberNode = node; }
    StateNode::MemberNode* getMemberNode(){return this->m_MemberNode;}

    void addDescriptor(std::string* string) { this->m_Descriptors.push_back(string); }
    std::vector<std::string*> getDescriptors(){return this->m_Descriptors;}

    void setFrame(StateNode::Frame* node) { this->m_Frame = node; }
    StateNode::Frame* getFrame(){return this->m_Frame;}

    void setCandidate(StateNode::Candidate* node) { m_Candidate = node; }
    StateNode::Candidate* getCandidate(){return this->m_Candidate;}

    void setDefinition(StateNode::Definition* node) { m_Definition = node; }
    StateNode::Definition* getDefinition(){return this->m_Definition;}

private:
    StateNode::Definition* m_Definition = nullptr;
    StateNode::Node* m_DefsNode = nullptr;
    StateNode::SyncNode* m_SyncNode = nullptr;
    StateNode::GroupNode* m_GroupNode = nullptr;
    StateNode::EventNode* m_EventNode = nullptr;
    StateNode::MemberNode* m_MemberNode = nullptr;
    StateNode::Frame* m_Frame = nullptr;
    StateNode::Candidate* m_Candidate = nullptr;
    std::vector<std::string*> m_Descriptors;

};

#endif // DEFSTREEWIDGETITEM_H
