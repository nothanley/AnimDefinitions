#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include "QFileDialog"
#include "QDebug"
#include "DefsTreeWidgetItem.h"
#include "DefsTableUtils.h"
#include "LabelTreeItemDelegate.h"
#pragma once

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString m_ExplorerPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Cache Explorer Directory

private slots:
    void on_actionOpen_File_triggered();
    void on_Button_File_Open_clicked();
    void on_TreeWidget_Defs_itemClicked(QTreeWidgetItem *item, int column);


    void on_expandcollapseButton_clicked(bool checked);

    void on_actionExpand_Collapse_triggered();

private:
    Ui::MainWindow *ui;
    void UI_ConstructDefsTree();
    void UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent);
    void UI_ConstructGroupNode(StateNode::GroupNode group, QTreeWidgetItem *parent);
    void UI_ConstructNewDefinitions(StateNode::Definition animDef);
    void UI_Table_BuildEVNT(StateNode::EventNode event);
    void UI_Table_BuildMEMB(StateNode::MemberNode member);
    void UI_Table_BuildCAND(StateNode::Candidate candidate);
    void UI_Table_BuildFRMS(StateNode::Frame frame);
    std::vector<StateNode::Definition> m_AnimDefinitions;
};

#endif // MAINWINDOW_H
