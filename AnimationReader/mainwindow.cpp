#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interface/C_DefInterface.h"
#include "Interface/C_TableBehavior.h"
#include "AnimDefinitions/C_DefinitionEncoder.h"

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


void MainWindow::UpdateFileStatsLabels(){
    quint64 fileSizeKB = QFileInfo(this->m_DefsFilePath).size() / 1024;
    ui->topStat_size->setText( QString::number(fileSizeKB) + " KB");
    ui->topStat_items->setText( QString::number(this->m_AnimDefinitions.size()) + " Defs");
    ui->topStat_info->setText("#");
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
    UpdateFileStatsLabels();
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


void MainWindow::on_SaveButton_clicked()
{
    if (this->m_AnimDefinitions.size() == 0){ return;}
    QString presetPath = (m_DefsFilePath == "") ? this->m_ExplorerPath : this->m_DefsFilePath;
    std::string saveFilePath = QFileDialog::getSaveFileName(this, tr("Save .adefs file"), presetPath,
                                        tr("Animation Definitions (*.adefs)") ).toStdString();
    if (saveFilePath == ""){ return; }
    qDebug() << "Save to " << QString::fromStdString(saveFilePath);
    CDefinitionEncoder(&m_AnimDefinitions).WriteToFile(saveFilePath);
}


void MainWindow::on_TreeWidget_Defs_itemClicked(QTreeWidgetItem *item, int column)
{
    DefsTreeWidgetItem *selectedItem = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (!selectedItem){return;}

    ClearAndResetTable(ui->TableWidget_Defs);
    ui->topStat_info->setText(item->text(0));
    updateTable = false;
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();
    switch (nodeType){
        case (STAT):
            ui->nodeTypeLabel->setText("STAT");
            CDefInterface::UI_Table_BuildKeyValue(ui->TableWidget_Defs,selectedItem);
            break;
        case (SYNC):
            ui->nodeTypeLabel->setText("SYNC");
            CDefInterface::UI_Table_BuildSYNC(ui->TableWidget_Defs,selectedItem);
            break;
        case (DESC):
            ui->nodeTypeLabel->setText("DESC");
            CDefInterface::UI_Table_BuildDESC(ui->TableWidget_Defs,selectedItem);
            break;
        case (EVNT):
            ui->nodeTypeLabel->setText("EVNT");
            CDefInterface::UI_Table_BuildEVNT(ui->TableWidget_Defs,selectedItem->getEventNode());
            break;
        case (MEMB):
            ui->nodeTypeLabel->setText("ITEM");
            CDefInterface::UI_Table_BuildMEMB(ui->TableWidget_Defs,selectedItem->getMemberNode());
            break;
        case (FRMS):
            ui->nodeTypeLabel->setText("FRMS");
            CDefInterface::UI_Table_BuildFRMS(ui->TableWidget_Defs,selectedItem);
            break;
        case (CAND):
            ui->nodeTypeLabel->setText("CAND");
            CDefInterface::UI_Table_BuildCAND(ui->TableWidget_Defs,selectedItem->getCandidate());
            break;
        case (NODE):
            ui->nodeTypeLabel->setText("NODE");
            CDefInterface::UI_Table_BuildNODE(ui->TableWidget_Defs,selectedItem);
            break;
        default:
            ui->nodeTypeLabel->setText("ITEM");
            CDefInterface::UI_Table_BuildKeyValue(ui->TableWidget_Defs,selectedItem);
            break;
    }
    ui->TableWidget_Defs->setItemDelegateForColumn(1,new ColorItemDelegate(this));
    updateTable = true;
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
{
    if (item->column() != 1){return;}
    DefsTreeWidgetItem *currentDef = dynamic_cast<DefsTreeWidgetItem*>(
                                                ui->TreeWidget_Defs->currentItem() );

    QString header = ui->TableWidget_Defs->item( item->row() , 0 )->text();
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();

    if (currentDef && updateTable){
        updateTable = false; //ignore-table refresh
        CTableBehavior::UpdateTableWithNode(currentDef,item,nodeType,header);
        updateTable = true;
    }
}


void MainWindow::on_TableWidget_Defs_cellDoubleClicked(int row, int column)
{
    updateTable = false; //ignore-table refresh
    QTableWidgetItem* item = ui->TableWidget_Defs->item(row,1);
    SyncedTableWidgetItem *syncItem = dynamic_cast<SyncedTableWidgetItem*>(item);
    QVariant prop = syncItem->getNodeProperty();

    if (prop.userType() == QMetaType::type("std::string *")){
        std::string* string = qvariant_cast<std::string*>(prop);
        item->setText(QString::fromStdString(*string));   }
    if (prop.userType() == QMetaType::type("uint64_t*")){
        uint64_t* num = qvariant_cast<uint64_t*>(prop);
        item->setText(QString::number(*num,16)); }
    if (prop.userType() == QMetaType::type("float*")){
        float* num = qvariant_cast<float*>(prop);
        item->setText(QString::number(*num,'f')); }
    if (prop.userType() == QMetaType::type("uint32_t*")){
        uint32_t* num = qvariant_cast<uint32_t*>(prop);
        item->setText(QString::number(*num)); }
    if (prop.userType() == QMetaType::type("QString*")){
        QString* num = qvariant_cast<QString*>(prop);
        item->setText(*num); }
    if (prop.userType() == QMetaType::type("uint16_t*")){
        uint16_t* num = qvariant_cast<uint16_t*>(prop);
        item->setText(QString::number(*num)); }
    if (prop.userType() == QMetaType::type("int32_t*")){
        int32_t* num = qvariant_cast<int32_t*>(prop);
        item->setText(QString::number(*num)); }
    if (prop.userType() == QMetaType::type("bool*")){
        bool* num = qvariant_cast<bool*>(prop);
        item->setText( (*num) ? "true" : "false"); }
    if (prop.userType() == QMetaType::type("std::string")){
        std::string num = qvariant_cast<std::string>(prop);
        item->setText( QString::fromStdString(num) ); }
    updateTable = true;
}









