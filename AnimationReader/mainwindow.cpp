#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Interface/C_DefInterface.h"
#include "Interface/C_TableBehavior.h"
#include "Encoder/dtmpserializer.h"
#include <QDragEnterEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->TreeWidget_Defs->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::on_TreeWidget_Defs_customContextMenuRequested(const QPoint &pos){
    QMenu menu(this);
    menu.setStyleSheet("QMenu::item:selected {background-color: rgb(81, 121, 194); color: rgb(225, 225, 225); }" );
    menu.addAction(ui->actionSave_Animation_Entry);
    menu.addAction(ui->actionAdd_Entry);
    menu.exec( ui->TreeWidget_Defs->mapToGlobal(pos) );
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString name;
    QList<QUrl> urls;
    QList<QUrl>::Iterator i;
    urls = event->mimeData()->urls();
    for(i = urls.begin(); i != urls.end(); ++i)
        name = i->toLocalFile();
    if (name != ""){ OpenFile(name); }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){event->acceptProposedAction();}


void ClearAndResetTable(QTableWidget* tableWidget){
    tableWidget->clearContents();
    tableWidget->setRowCount(0); }

void CollapseAllEventItems( QTreeWidget* tree, QTreeWidgetItem* item){
    QVariant type = item->data(0,Qt::UserRole+1);
    if (type.isValid() && type.toUInt())
        if (type == EVNT){ item->setExpanded(false); }
    for( int i = 0; i < item->childCount(); ++i )
        CollapseAllEventItems(tree,item->child(i));
}

void MainWindow::UpdateFileStatsLabels(){
    quint64 fileSizeKB = QFileInfo(this->m_DefsFilePath).size() / 1024;
    ui->topStat_size->setText( QString::number(fileSizeKB) + " KB");
    ui->topStat_items->setText( QString::number(this->m_AnimDefinitions->size()) + " Defs");
    ui->topStat_info->setText("#");
}

void MainWindow::OpenFile(QString path){
    if (!path.endsWith(".adefs")){return;}
    this->m_DefsFilePath = path;
    this->animFile = new ADefHandler( path.toStdString().c_str() );
    this->m_AnimDefinitions = &animFile->m_Definitions; // Read Definitions file- collects all defs
    this->m_binaryStream = &animFile->m_metaStream;
    CDefInterface::UI_ConstructDefsTree(ui->TreeWidget_Defs,ui->TableWidget_Defs,m_AnimDefinitions);
    UpdateWindowData();
    UpdateFileStatsLabels();
}

void MainWindow::on_actionOpen_File_triggered()
{
    this->m_DefsFilePath = QFileDialog::getOpenFileName(this, tr("Open .adefs file"),
                                                     this->m_ExplorerPath,
                                                     tr("Animation Definitions (*.adefs)") );
    if (m_DefsFilePath == ""){ return; }
    OpenFile(m_DefsFilePath);
}


void MainWindow::UpdateWindowData(){
    ui->Label_Top_Path->setText(this->m_DefsFilePath);
    ui->TreeStats->setText("Total Defs: " + QString::number(m_AnimDefinitions->size()) );
    this->ui->expandcollapseButton->setChecked(false);
    on_expandcollapseButton_clicked(false);
    LabelTreeItemDelegate* labelDelegate = new LabelTreeItemDelegate();
    labelDelegate->setTreeWidget(ui->TreeWidget_Defs);
    ui->TreeWidget_Defs->setItemDelegate( labelDelegate );
}

void MainWindow::on_Button_File_Open_clicked(){ /* Redirect to File Open Method */ on_actionOpen_File_triggered(); }


QString getDirectoryPath(QString filePath){
    if (filePath.count("/") == 0){return filePath;}
    QStringList paths = filePath.split("/");
    paths.removeLast();
    return paths.join("/");
}

void MainWindow::on_SaveButton_clicked(){
    on_actionSaveToCurrentFile_triggered();
}

void MainWindow::on_expandcollapseButton_clicked(bool checked){
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

void MainWindow::on_TableWidget_Defs_itemChanged(QTableWidgetItem *item){
    if (item->column() != 1){return;}
    DefsTreeWidgetItem *currentDef = dynamic_cast<DefsTreeWidgetItem*>(
                                                ui->TreeWidget_Defs->currentItem() );
    if (!item || !currentDef){return;}
    QString header = ui->TableWidget_Defs->item( item->row() , 0 )->text();
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();
    if (currentDef && updateTable){
        updateTable = false; //ignore-table refresh
        CTableBehavior::UpdateTableWithNode(currentDef,item,nodeType,header);
        updateTable = true; }
}


void MainWindow::on_actionSave_Definitions_triggered(){
    if (!isDefLoaded()){
        QMessageBox::warning(this,"Warning", "Please load a definition to save file."); return;}
    QString presetPath = (m_DefsFilePath == "") ? this->m_ExplorerPath : getDirectoryPath(this->m_DefsFilePath);
    std::string saveFilePath = QFileDialog::getSaveFileName(this, tr("Save .adefs file"), presetPath,
                                        tr("Animation Definitions (*.adefs)") ).toStdString();
    if (saveFilePath == ""){ return; }
    CDefinitionEncoder(m_AnimDefinitions).WriteToFileWithStream(saveFilePath,this->m_binaryStream);
    QMessageBox::warning(this, "Save Complete", "File Saved Successfully to: \""
                         + QString::fromStdString(saveFilePath) + "\"");
}


void MainWindow::on_actionExit_triggered(){ QApplication::exit(); }


void MainWindow::on_actionPrint_definition_items_triggered(){
    for (auto &def : *this->m_AnimDefinitions)
        for (auto &stat : def.stateNodes)
            qDebug() << " ITEM DEF: " << QString::fromStdString(stat.childNodes[0].keyValueProperties[1].chars);
}


void HighlightItem( QTreeWidget* tree, QTreeWidgetItem *item ){
    tree->itemClicked(item,0);
    for( int i = 0; i < item->childCount(); ++i )
        HighlightItem(tree,item->child(i));
}

void MainWindow::on_actionClick_Every_Item_Once_triggered(){
    HighlightItem( ui->TreeWidget_Defs, ui->TreeWidget_Defs->invisibleRootItem() );
}

QTreeWidgetItem* SearchForItem( QTreeWidget* tree, QString elementString ){
    QList<QTreeWidgetItem*> items = tree->findItems(elementString,Qt::MatchContains|Qt::MatchRecursive);
    if (items.size() == 0){return nullptr;}
    return items.at(0);
}

void selectItem( QTreeWidget* tree, QTreeWidgetItem *item ){
    tree->clearSelection();
    item->setSelected(true);
    tree->scrollToItem(item);
}

void MainWindow::on_SearchButton_clicked(){
    QString searchString = ui->SearchLine->text();
    if (searchString == ""){return;}
    QTreeWidgetItem* item = SearchForItem(ui->TreeWidget_Defs,searchString);
    if (!item){ return; }
    selectItem(ui->TreeWidget_Defs,item);
}


void MainWindow::on_SearchLine_returnPressed(){ on_SearchButton_clicked(); }

void SetItemVisibility(QTreeWidgetItem*item, bool isHidden){
    if (item->data(0,Qt::UserRole+1) == EVNT)
        item->setHidden(isHidden);
    for (int i = 0; i < item->childCount(); i++){
        SetItemVisibility(item->child(i),isHidden); }
}

QTreeWidgetItem* GetItemParent(QTreeWidgetItem* item){
    QTreeWidgetItem* parent = item;
    while (parent->parent() != NULL)
        parent = parent->parent();
    return parent;
}

void MainWindow::on_TreeWidget_Defs_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous){
    if (current == NULL || previous == NULL){return;}
    QTreeWidgetItem* currentItemParent = GetItemParent(current);
    QTreeWidgetItem* previousItemParent = GetItemParent(previous);

    if (currentItemParent != previousItemParent){
        SetItemVisibility(currentItemParent,false);
        SetItemVisibility(previousItemParent,true);
        ui->TreeWidget_Defs->scrollToItem(currentItemParent); }
}

void CreateTemplateInputWindow(QWidget* parent, QString* menuInput, bool* ok){
    *menuInput = QInputDialog::getText(parent,("New Material Template"), ("Enter Template Name:"), QLineEdit::Normal,"", ok);
}

void MainWindow::AddTemplateDialogPress(Definition definition){
    if (!this->m_AnimDefinitions){
        QMessageBox::warning(this,"Warning", "Please load .adefs file to add nodes.");
        return;}
    this->m_AnimDefinitions->push_back(definition);
    int index = m_AnimDefinitions->size();
    CDefInterface::UI_ConstructNewDefinition(ui->TreeWidget_Defs,&m_AnimDefinitions->back(),index);
    qDebug() << "Add complete.";
}

void MainWindow::on_pushButton_clicked()
{
    NodeDialog* templateDialog = new NodeDialog(this);
    connect(templateDialog, &NodeDialog::AddTemplateClicked, this, &MainWindow::AddTemplateDialogPress);
    templateDialog->setVisible(true);
}

bool isValidTreeItem(QTreeWidgetItem* item){
    item = GetItemParent(item);
    if (!item || item == NULL){ return false;}
    DefsTreeWidgetItem *selectedItem = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (!selectedItem){return false;}
    return selectedItem->text(0).contains("Definition #");
}

void SaveTreeStateToDTMP(QTreeWidgetItem* item, QString templateName){
    item = GetItemParent(item);
    DefsTreeWidgetItem *selectedItem = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (selectedItem->getDefinition() == nullptr){ qDebug() << "No Def found."; return;}

    DtmpSerializer file("anim_templates.dtmp");
    file.AppendDefinition(selectedItem->getDefinition(),templateName.toStdString().c_str());
    file.WriteContents();
}

void MainWindow::on_actionSave_Animation_Entry_triggered(){
    if (!this->m_AnimDefinitions){
        QMessageBox::warning(this,"Warning", "Please load .adefs file to save def.");
        return;}
    if (!ui->TreeWidget_Defs->currentItem()){
        QMessageBox::warning(this,"Warning", "Please load node to save def.");
        return;}
    QTreeWidgetItem* item = ui->TreeWidget_Defs->selectedItems().first();
    if (!isValidTreeItem(item)){return;}
    QString templateName = ""; bool ok;
    CreateTemplateInputWindow(this,&templateName,&ok);
    if (!ok || templateName.isEmpty()) { return;};
    SaveTreeStateToDTMP(item,templateName);
}


void MainWindow::on_actionAdd_Entry_triggered()
{
    on_pushButton_clicked();
}




void MainWindow::on_TableWidget_Defs_cellDoubleClicked(int row, int column)
{
    updateTable = false; //ignore-table refresh
    QTableWidgetItem* item = ui->TableWidget_Defs->item(row,1);
    SyncedTableWidgetItem *syncItem = dynamic_cast<SyncedTableWidgetItem*>(item);
    if (!item || !syncItem){return;}
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



void CreateSaveOverwritePrompt(QString path, bool* ok){
    QMessageBox msgBox;
    msgBox.setWindowTitle("Overwrite File");
    msgBox.setText("Are you sure you want to overwrite " + path+ "?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes){*ok = true;}
    else { *ok = false; }
}


void MainWindow::on_actionSaveToCurrentFile_triggered()
{
    if (m_DefsFilePath == ""){ on_actionSave_Definitions_triggered(); return;}
    if (!isDefLoaded()){
        QMessageBox::warning(this,"Warning", "Please load a definition to save file."); return;}
    QString savePath = m_DefsFilePath;
    bool isSaveReady = false;
    CreateSaveOverwritePrompt(savePath,&isSaveReady);
    if (isSaveReady){
        CDefinitionEncoder(m_AnimDefinitions).WriteToFileWithStream(savePath.toStdString(),this->m_binaryStream);
        QMessageBox::warning(this, "Save Complete", "File Saved Successfully to: \""
                             + savePath + "\""); }
}









