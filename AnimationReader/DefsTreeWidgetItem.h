/* Subclass for custom TreeWidgetItem's enabling storing additional required data for animation definitions */
#ifndef DEFSTREEWIDGETITEM_H
#define DEFSTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "ColorItemDelegate.h"
#include "AnimDefinitions/AnimDefHandler.h"
#pragma once

class DefsTreeWidgetItem : public QTreeWidgetItem
{
public:
    DefsTreeWidgetItem(QTreeWidget *treeview, int type = Type)  : QTreeWidgetItem(treeview,type) {};
    DefsTreeWidgetItem(QTreeWidgetItem *parent, int type = Type) : QTreeWidgetItem(parent,type) {};
    void setStateNode(StateNode::Node node) { m_DefsNode = node; }
    StateNode::Node getStateNode(){return this->m_DefsNode;}
private:
    StateNode::Node m_DefsNode;
};

#endif // DEFSTREEWIDGETITEM_H
