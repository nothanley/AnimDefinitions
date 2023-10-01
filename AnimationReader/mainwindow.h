#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include "QFileDialog"
#include "Widget/DefsTreeWidgetItem.h"
#include "Widget/DefsTableUtils.h"
#include "Widget/LabelTreeItemDelegate.h"
#include "nodedialog.h"
#include <QInputDialog>
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
    void UpdateWindowData();
    void on_TableWidget_Defs_itemChanged(QTableWidgetItem *item);
    void RefreshTableTreeSync();
    void on_TableWidget_Defs_cellDoubleClicked(int row, int column);
    void UpdateFileStatsLabels();
    void on_SaveButton_clicked();
    void on_actionSave_Definitions_triggered();
    void on_actionExit_triggered();
    void on_actionPrint_definition_items_triggered();
    void on_actionClick_Every_Item_Once_triggered();
    void on_SearchButton_clicked();
    void on_SearchLine_returnPressed();
    void on_TreeWidget_Defs_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_pushButton_clicked();
    void on_TreeWidget_Defs_customContextMenuRequested(const QPoint &pos);
    void on_actionSave_Animation_Entry_triggered();
    void on_actionAdd_Entry_triggered();
    void AddTemplateDialogPress( StateNode::Definition definition);

private:
    Ui::MainWindow *ui;
    ADefHandler* animFile;
    std::vector<StateNode::Definition>* m_AnimDefinitions = nullptr;
    std::vector<char>* m_binaryStream;
    QString m_DefsFilePath = "";
    bool updateTable = false;


};

#endif // MAINWINDOW_H
