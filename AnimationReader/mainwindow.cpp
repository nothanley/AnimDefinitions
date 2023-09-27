#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interface/C_DefInterface.h"
#include "Interface/C_TableBehavior.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    QFont labelFont("Microsoft New PhagsPa",7);
    ui->Label_Top_Path->setFont(labelFont);
    delete ui;
}

void ClearAndResetTable(QTableWidget* tableWidget)
{
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    this->m_DefsFilePath = QFileDialog::getOpenFileName(this, tr("Open .adefs file"),
                                                     this->m_ExplorerPath,
                                                     tr("Animation Definitions (*.adefs)") );
    if (m_DefsFilePath == ""){ return; }
    this->m_AnimDefinitions = ADefHandler( m_DefsFilePath.toStdString().c_str() ).m_Definitions; // Read Definitions file- collects all defs
    CDefInterface::UI_ConstructDefsTree(ui->TreeWidget_Defs,ui->TableWidget_Defs,m_AnimDefinitions);
    UpdateWindowData();
}

void MainWindow::UpdateWindowData(){
    ui->Label_Top_Path->setText(this->m_DefsFilePath);
    ui->TreeStats->setText("Total Defs: " + QString::number(m_AnimDefinitions.size()) );
    if (ui->expandcollapseButton->isChecked()){ui->TreeWidget_Defs->expandAll();}
    else{ ui->TreeWidget_Defs->collapseAll();}
    LabelTreeItemDelegate* labelDelegate = new LabelTreeItemDelegate();
    labelDelegate->setTreeWidget(ui->TreeWidget_Defs);
    ui->TreeWidget_Defs->setItemDelegate( labelDelegate );
}

void MainWindow::on_Button_File_Open_clicked(){ /* Redirect to File Open Method */ on_actionOpen_File_triggered(); }


void MainWindow::on_TreeWidget_Defs_itemClicked(QTreeWidgetItem *item, int column)
{
    DefsTreeWidgetItem *selectedItem = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (!selectedItem){return;}

    ClearAndResetTable(ui->TableWidget_Defs);
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();
    switch (nodeType){
        case (SYNC):
            CDefInterface::UI_Table_BuildSYNC(ui->TableWidget_Defs,selectedItem);
            break;
        case (DESC):
            CDefInterface::UI_Table_BuildDESC(ui->TableWidget_Defs,selectedItem);
            break;
        case (EVNT):
            CDefInterface::UI_Table_BuildEVNT(ui->TableWidget_Defs,selectedItem->getEventNode());
            break;
        case (MEMB):
            CDefInterface::UI_Table_BuildMEMB(ui->TableWidget_Defs,selectedItem->getMemberNode());
            break;
        case (FRMS):
            CDefInterface::UI_Table_BuildFRMS(ui->TableWidget_Defs,selectedItem);
            break;
        case (CAND):
            CDefInterface::UI_Table_BuildCAND(ui->TableWidget_Defs,selectedItem->getCandidate());
            break;
        case (NODE):
            CDefInterface::UI_Table_BuildNODE(ui->TableWidget_Defs,selectedItem);
            break;
        default:
            CDefInterface::UI_Table_BuildKeyValue(ui->TableWidget_Defs,selectedItem);
            break;
    }
    ui->TableWidget_Defs->setItemDelegateForColumn(1,new ColorItemDelegate(this));
}


void MainWindow::on_expandcollapseButton_clicked(bool checked)
{
    if (checked){
        this->ui->TreeWidget_Defs->expandAll();
        this->ui->expandcollapseButton->setText("Collapse All");
        return; }
    this->ui->TreeWidget_Defs->collapseAll();
    this->ui->expandcollapseButton->setText("Expand All");
}


void MainWindow::on_actionExpand_Collapse_triggered()
{
    bool isToggled =  ui->expandcollapseButton->isChecked();
    ui->expandcollapseButton->setChecked(!isToggled);
    on_expandcollapseButton_clicked( !isToggled );
}

void MainWindow::RefreshTableTreeSync(){
    on_TreeWidget_Defs_itemClicked(ui->TreeWidget_Defs->currentItem(), 0);
}

void MainWindow::on_TableWidget_Defs_itemChanged(QTableWidgetItem *item)
{/*
    if (item->column() != 1){return;}
    DefsTreeWidgetItem *currentDef = dynamic_cast<DefsTreeWidgetItem*>(
                                                ui->TreeWidget_Defs->currentItem() );
    QString header = ui->TableWidget_Defs->item( item->row() , 0 )->text();
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();
    if (currentDef && !ignoreTableUpdate){
        qDebug() << "\nUsing Def:" << currentDef->text(0);
        qDebug() << "Node Type: " << nodeType;
        CTableBehavior::UpdateTableWithNode(currentDef,item,nodeType,header);
    }*/
}


void MainWindow::on_TableWidget_Defs_cellDoubleClicked(int row, int column)
{
    QTableWidgetItem* item = ui->TableWidget_Defs->item(row,column);
    if (item->text().contains("\"")){
        item->setText(item->text().remove("\""));
    }
}

