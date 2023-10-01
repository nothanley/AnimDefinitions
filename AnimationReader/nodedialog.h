#ifndef NODEDIALOG_H
#define NODEDIALOG_H
#include "Encoder/dtmpserializer.h"
#include <QDialog>

namespace Ui {
class NodeDialog;
}

class NodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeDialog(QWidget *parent = nullptr);
    ~NodeDialog();

private slots:
    void on_buttonBox_accepted();

    void on_NodeDialog_accepted();

    void on_NodeDialog_finished(int result);

    void on_buttonBox_rejected();

private:
    DtmpSerializer *m_TemplatesFile;
    Ui::NodeDialog *ui;

signals:
    void AddTemplateClicked(StateNode::Definition definition);
};



#endif // NODEDIALOG_H
