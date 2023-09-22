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
        this->m_AnimDefinitions = ADefHandler( adefsPath.toStdString().c_str() ); // Read Definitions file- collects all nodes
        ui->Label_Top_Path->setText(QString("  " + adefsPath));
        UI_ConstructDefsTree();
    }
}

void MainWindow::on_Button_File_Open_clicked(){ /* Redirect to File Open Method */ on_actionOpen_File_triggered(); }

void MainWindow::UI_ConstructDefsTree()
{
    ui->TreeWidget_Defs->clear();
    uint32_t totalNodes = m_AnimDefinitions.m_Definitions.size();
    for (int i = 0; i < totalNodes; i++){
        /*setups node*/
        StateNode::Node animationNode = m_AnimDefinitions.m_Definitions[i];
        UI_ConstructStateNode(animationNode,nullptr);
    }
    qDebug() << "[DEBUG]" << totalNodes << " Defs Loaded.";
}


void MainWindow::UI_ConstructStateNode(StateNode::Node node, QTreeWidgetItem *parent)
{
    DefsTreeWidgetItem *item = nullptr;
    if (parent){ item = new DefsTreeWidgetItem(parent);}
    else{ item = new DefsTreeWidgetItem(ui->TreeWidget_Defs);}

    item->setText(0, parent ? "Child Node" : "Parent Node");
    item->setStateNode(node);
    if (node.childNodes.size() != 0)
        for (const auto &childNode:node.childNodes )
            UI_ConstructStateNode(childNode,item);
}


void MainWindow::on_TreeWidget_Defs_itemClicked(QTreeWidgetItem *item, int column)
{
    DefsTreeWidgetItem *itemTest = dynamic_cast<DefsTreeWidgetItem*>(item);
    if (!itemTest){return;}

    int itemCount = 0;
    ui->TableWidget_Defs->clearContents();
    StateNode::Node currentTreeNode = itemTest->getStateNode();
    qDebug() << "[DEBUG]" << currentTreeNode.keyValueProperties.size() << " KV Sets.";

    for (const auto& keyValueSet : currentTreeNode.keyValueProperties)
    {
        for (int i = 0; i < keyValueSet.size(); i++)
        {
            itemCount++;
            QTableWidgetItem *tableItemKey = new QTableWidgetItem( UI_FormatKeyValueString(keyValueSet[i]) );
            QTableWidgetItem *tableItemValue = new QTableWidgetItem( UI_FormatKeyValueString(keyValueSet[i],true) );
            ui->TableWidget_Defs->setRowCount( itemCount );
            ui->TableWidget_Defs->setItem(itemCount-1,0,tableItemKey);
            ui->TableWidget_Defs->setItem(itemCount-1,1,tableItemValue);
        }
    }

    ui->TableWidget_Defs->setItemDelegateForColumn(1,new ColorItemDelegate(this));
}








