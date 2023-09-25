#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_actionOpen_File_triggered()
{
    QString adefsPath = QFileDialog::getOpenFileName(this, tr("Open .adefs file"),
                                                     this->m_ExplorerPath,
                                                     tr("Animation Definitions (*.adefs)") );
    if (adefsPath == ""){ return; }
    else
    {
        this->m_AnimDefinitions = ADefHandler( adefsPath.toStdString().c_str() ).m_Definitions; // Read Definitions file- collects all defs
        ui->Label_Top_Path->setText(QString(adefsPath));
        UI_ConstructDefsTree();

        LabelTreeItemDelegate* labelDelegate = new LabelTreeItemDelegate();
        labelDelegate->setTreeWidget(ui->TreeWidget_Defs);
//        labelDelegate->style

        ui->TreeWidget_Defs->setItemDelegate( labelDelegate );
//        ui->TreeWidget_Defs->setContentsMargins(0,10,10,0);
    }
}

void MainWindow::on_Button_File_Open_clicked(){ /* Redirect to File Open Method */ on_actionOpen_File_triggered(); }

void MainWindow::UI_ConstructDefsTree()
{
    ui->TreeWidget_Defs->clear();
    uint32_t totalDefs = m_AnimDefinitions.size();

    for (int i = 0; i < totalDefs; i++)
       UI_ConstructNewDefinitions(m_AnimDefinitions[i]);

    ui->TreeStats->setText("Total Defs: " + QString::number(totalDefs) );
    if (ui->expandcollapseButton->isChecked()){ ui->TreeWidget_Defs->expandAll();}
    else{ ui->TreeWidget_Defs->collapseAll();
    }
    qDebug() << "[DEBUG]" << totalDefs << " Defs Loaded.";
}


void MainWindow::UI_ConstructNewDefinitions(StateNode::Definition animDef){

    DefsTreeWidgetItem *rootParent = new DefsTreeWidgetItem(ui->TreeWidget_Defs);

    //Construct State sets
    for (const auto &stateNode:animDef.stateNodes )
        UI_ConstructStateNode(stateNode,rootParent);

    //Construct Group sets
    for (const auto &groupNode:animDef.groupNodes )
        UI_ConstructGroupNode(groupNode,rootParent);

    rootParent->setText(0, "Definition Item");

}

void MainWindow::UI_ConstructGroupNode(StateNode::GroupNode group, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, "GROUP: " + QString(group.chars.c_str()) );
    item->setData(0,Qt::UserRole+1, GRP_ );
    item->setGroupNode(group);

    //todo: EVERYTHING !!

    //Construct DESC sets
    for (const auto &descriptor :group.descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(item);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(descriptor);
    }

    //Construct Memb sets
    {
        DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(item);
        membItem->setText(0, "MEMB Item");
        membItem->setData(0,Qt::UserRole+1, MEMB );
        // membItem->setMemberNode(group.members);
        for (const auto &frame :group.members.frames ){
            DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
            frmsItem->setText(0, "FRMS Item");
            frmsItem->setData(0,Qt::UserRole+1, FRMS );
            // frmsItem->setMemberNode(group.members);
        }
        for (const auto &candidate :group.members.candidates ){
            DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
            candItem->setText(0, "FRMS Item");
            candItem->setData(0,Qt::UserRole+1, CAND );
            // candItem->setMemberNode(group.members);
        }
    }

    //Construct SELS sets
    {
        DefsTreeWidgetItem *selsItem = new DefsTreeWidgetItem(item);
        selsItem->setText(0, "SELS Item");
        selsItem->setData(0,Qt::UserRole+1, SELS );
        for (const auto &member :group.selectors.members ){
            DefsTreeWidgetItem *membItem = new DefsTreeWidgetItem(selsItem);
            membItem->setText(0, "MEMB Item");
            membItem->setData(0,Qt::UserRole+1, MEMB );
            // membItem->setMemberNode(group.members);
            for (const auto &frame :group.members.frames ){
                DefsTreeWidgetItem *frmsItem = new DefsTreeWidgetItem(membItem);
                frmsItem->setText(0, "FRMS Item");
                frmsItem->setData(0,Qt::UserRole+1, FRMS );
                // frmsItem->setMemberNode(group.members);
            }
            for (const auto &candidate :group.members.candidates ){
                DefsTreeWidgetItem *candItem = new DefsTreeWidgetItem(membItem);
                candItem->setText(0, "FRMS Item");
                candItem->setData(0,Qt::UserRole+1, CAND );
                // candItem->setMemberNode(group.members);
            }
        }
    }
}

void MainWindow::UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = new DefsTreeWidgetItem(parent);
    item->setText(0, (node.nodeType == STAT) ? "STAT Node" : "Child Node");
    item->setStateNode(node);
    item->setData(0,Qt::UserRole+1, (node.nodeType == STAT) ? STAT : NODE );

    //Construct NODE sets
    for (const auto &childNode:node.childNodes )
        UI_ConstructStateNode(childNode,item);

    //Construct SYNC sets
    for (const auto &syncNode :node.syncNodes ){
        DefsTreeWidgetItem *syncItem = new DefsTreeWidgetItem(item);
        syncItem->setText(0, "SYNC Item");
        syncItem->setSyncNode(syncNode);
        syncItem->setData(0,Qt::UserRole+1, SYNC );
    }

    //Construct DTT sets
    for (const auto &dttNode :node.transNodes ){
        DefsTreeWidgetItem *dttItem = new DefsTreeWidgetItem(item);
        dttItem->setText(0, "DTT_ Item");
        dttItem->setStateNode(dttNode);
        dttItem->setData(0,Qt::UserRole+1, DTT_ );
    }

    //Construct TOVR sets
    for (const auto &ovlyNode :node.ovlyNodes ){
        DefsTreeWidgetItem *ovlyItem = new DefsTreeWidgetItem(item);
        ovlyItem->setText(0, "TOVR Item");
        ovlyItem->setStateNode(ovlyNode);
        ovlyItem->setData(0,Qt::UserRole+1, TOVR );
    }

    //Construct DESC sets
    for (const auto &descriptor :node.descriptors ){
        DefsTreeWidgetItem *descItem = new DefsTreeWidgetItem(item);
        descItem->setText(0, "DESC Item");
        descItem->setData(0,Qt::UserRole+1, DESC );
        descItem->addDescriptor(descriptor);
    }

    //Construct Event sets
    for ( int i = 0; i < node.events.size(); i++ ){
        DefsTreeWidgetItem *evntItem = new DefsTreeWidgetItem(item);
        evntItem->setText(0, "Event #" + QString::number(i) );
        evntItem->setData(0,Qt::UserRole+1, EVNT );
        evntItem->setEventNode(node.events[i]);
    }
}


void MainWindow::on_TreeWidget_Defs_itemClicked(QTreeWidgetItem *item, int column)
{
    DefsTreeWidgetItem *itemTest = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (!itemTest){return;}

    int itemCount = 0;
    ui->TableWidget_Defs->clearContents();
    uint32_t nodeType = ui->TreeWidget_Defs->currentIndex().data(Qt::UserRole+1).toUInt();
    switch (nodeType){
        case (SYNC):
            {
                itemCount+=3;
                ui->TableWidget_Defs->setRowCount( itemCount );
                StateNode::SyncNode sync = itemTest->getSyncNode();
                ui->TableWidget_Defs->setItem(itemCount-3,0, new QTableWidgetItem( "\"flag\" :" ));
                ui->TableWidget_Defs->setItem(itemCount-3,1,new QTableWidgetItem( sync.flag ? "true" : "false" ));
                ui->TableWidget_Defs->setItem(itemCount-2,0, new QTableWidgetItem( "\"value 0\" :" ));
                ui->TableWidget_Defs->setItem(itemCount-2,1,new QTableWidgetItem( "\"" + QString::number(sync.valueA,16) + "\""));
                ui->TableWidget_Defs->setItem(itemCount-1,0, new QTableWidgetItem( "\"value 1\" :" ));
                ui->TableWidget_Defs->setItem(itemCount-1,1,new QTableWidgetItem( "\"" +QString::number(sync.valueB,16) + "\""));
            }
            break;
        case (DESC):
            {
                std::vector<std::string> descriptors = itemTest->getDescriptors();
                for (int i = 0; i < descriptors.size(); i++){
                     itemCount++;
                     QTableWidgetItem *tableItemKey = new QTableWidgetItem( "\"desc\" :" );
                     QTableWidgetItem *tableItemValue = new QTableWidgetItem( QString(descriptors[i].c_str()) );
                     ui->TableWidget_Defs->setRowCount( itemCount );
                     ui->TableWidget_Defs->setItem(itemCount-1,0,tableItemKey);
                     ui->TableWidget_Defs->setItem(itemCount-1,1,tableItemValue);
                 }
            }
            break;
        case (EVNT):
            UI_Table_BuildEVNT( itemTest->getEventNode() );
            break;
        default:
            for (const auto& keyValueSet : itemTest->getStateNode().keyValueProperties)
                for (int i = 0; i < keyValueSet.size(); i++){
                    itemCount++;
                    QTableWidgetItem *tableItemKey = new QTableWidgetItem( UI_FormatKeyValueString(keyValueSet[i]) );
                    QTableWidgetItem *tableItemValue = new QTableWidgetItem( UI_FormatKeyValueString(keyValueSet[i],true) );
                    ui->TableWidget_Defs->setRowCount( itemCount );
                    ui->TableWidget_Defs->setItem(itemCount-1,0,tableItemKey);
                    ui->TableWidget_Defs->setItem(itemCount-1,1,tableItemValue);    }
        break;
    }

    ui->TableWidget_Defs->setItemDelegateForColumn(1,new ColorItemDelegate(this));
}


void MainWindow::UI_Table_BuildEVNT(StateNode::EventNode event){
    int itemCount = 0;

    itemCount+=2;
    ui->TableWidget_Defs->setRowCount( itemCount );
    ui->TableWidget_Defs->setItem(itemCount-2,0, new QTableWidgetItem( "\"value_0\" :" ));
    ui->TableWidget_Defs->setItem(itemCount-2,1,new QTableWidgetItem(  "\"" + QString::number(event.value_0,16) + "\"") );

    ui->TableWidget_Defs->setItem(itemCount-1,0, new QTableWidgetItem( "\"value 1\" :" ));
    ui->TableWidget_Defs->setItem(itemCount-1,1,new QTableWidgetItem( QString::number(event.value_1) ) );

    if (event.flag){
        itemCount+=2;
        ui->TableWidget_Defs->setRowCount( itemCount );
        ui->TableWidget_Defs->setItem(itemCount-2,0, new QTableWidgetItem( "\"value_2\" :" ));
        ui->TableWidget_Defs->setItem(itemCount-2,1,new QTableWidgetItem( QString::number(event.value_2) ) );
        ui->TableWidget_Defs->setItem(itemCount-1,0, new QTableWidgetItem( "\"value_3\" :" ));
        ui->TableWidget_Defs->setItem(itemCount-1,1,new QTableWidgetItem( QString::number(event.value_3,16)) );
    }

    for (int i = 0; i < event.triggers.size(); i++){
        itemCount+=2;
        ui->TableWidget_Defs->setRowCount( itemCount );
        ui->TableWidget_Defs->setItem(itemCount-2,0, new QTableWidgetItem( "\"trig_" + QString::number(i) + "\" :" ));
        ui->TableWidget_Defs->setItem(itemCount-2,1,new QTableWidgetItem( "\"" + QString::number(event.triggers[i].hash,16) + "\"") );
        ui->TableWidget_Defs->setItem(itemCount-1,0, new QTableWidgetItem( "\"trig_" + QString::number(i) + " value\" :" ));
        ui->TableWidget_Defs->setItem(itemCount-1,1,new QTableWidgetItem( QString::number(event.triggers[i].value) ) );
    }

    for (int i = 0; i < event.arguments.size(); i++){
        itemCount++;
        ui->TableWidget_Defs->setRowCount( itemCount );
        ui->TableWidget_Defs->setItem(itemCount-1,0, new QTableWidgetItem( "\"arg_" + QString::number(i) + "\" :" ));
        ui->TableWidget_Defs->setItem(itemCount-1,1,new QTableWidgetItem( QString::number(event.arguments[i].index) ) );
    }

}






void MainWindow::on_expandcollapseButton_clicked(bool checked)
{
    if (checked){
        this->ui->TreeWidget_Defs->expandAll();
        this->ui->expandcollapseButton->setText("Collapse All");
    }
    else{
        this->ui->TreeWidget_Defs->collapseAll();
        this->ui->expandcollapseButton->setText("Expand All");
    }
}


void MainWindow::on_actionExpand_Collapse_triggered()
{
    bool isToggled =  ui->expandcollapseButton->isChecked();
    ui->expandcollapseButton->setChecked(!isToggled);
    on_expandcollapseButton_clicked( !isToggled );
}

