#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardPaths>
#include "QFileDialog"
#include "QDebug"
#include "DefsTreeWidgetItem.h"
#include "DefsTableUtils.h"
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

private:
    Ui::MainWindow *ui;
    void UI_ConstructDefsTree();
    void UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent);
    ADefHandler m_AnimDefinitions;
};
#endif // MAINWINDOW_H
