#include "nodedialog.h"
#include "ui_nodedialog.h"
#include <QDebug>

NodeDialog::NodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeDialog)
{
    ui->setupUi(this);

    this->m_TemplatesFile = new DtmpSerializer("anim_templates.dtmp");
    if (m_TemplatesFile->isValid && m_TemplatesFile->numTemplates != 0){
        for (const auto &entry : m_TemplatesFile->getTemplates())
            ui->comboBox->addItem(QString::fromStdString(entry.name));  }
}

NodeDialog::~NodeDialog()
{
    delete ui;
}


DefEntry* FindMatchingEntry(std::vector<DefEntry>* templates, std::string name){
    for (auto &entry : *templates)
       if (entry.name == name){
           return &entry;}
    return nullptr;
}

StateNode::Definition ReadDefinitionStream(std::stringstream* stream){
    std::vector<StateNode::Definition> nodeArray;
    stream->seekg(0);
    C_StateNode state(stream);
    state.InitializeDefinitions(nodeArray);
    return nodeArray.at(0);
}

void NodeDialog::on_buttonBox_accepted()
{
    StateNode::Definition definition;
    std::vector<DefEntry> templates = this->m_TemplatesFile->getTemplates();
    DefEntry* definitionEntry = FindMatchingEntry(&templates, ui->comboBox->currentText().toStdString());
    if (!definitionEntry){return;}

    // Initialize definition
    if (definitionEntry->stream.size() != 0){
        std::stringstream stream;
        stream.write(definitionEntry->stream.data(),definitionEntry->size);
        definition = ReadDefinitionStream(&stream);
    }
    // If valid add it to main application
    if (definition.stateNodes.size() == 0 ||
            definition.groupNodes.size() == 0){
        AddTemplateClicked(definition); }
}

void NodeDialog::on_NodeDialog_accepted(){}

void NodeDialog::on_NodeDialog_finished(int result){}

void NodeDialog::on_buttonBox_rejected(){ }



