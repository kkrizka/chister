#include "SwissHCC_ConfigureForm.h"
#include "ui_SwissHCC_ConfigureForm.h"

#include <QFileDialog>

SwissHCC_ConfigureForm::SwissHCC_ConfigureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwissHCC_ConfigureForm)
{
    ui->setupUi(this);
}

SwissHCC_ConfigureForm::~SwissHCC_ConfigureForm()
{
    delete ui;
}

void SwissHCC_ConfigureForm::setTemplates(const QStringList& templates)
{
  for(int i=0;i<ui->templateComboBox->count();i++) ui->templateComboBox->removeItem(0);
  for(const QString& name : templates)
    ui->templateComboBox->addItem(name);
}

void SwissHCC_ConfigureForm::on_logPushButton_clicked()
{
    m_logDirectory = QFileDialog::getExistingDirectory(this, tr("Select Log Directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly);
    ui->logPushButton->setText(QString("Log: %1").arg(m_logDirectory));
}

void SwissHCC_ConfigureForm::on_donePushButton_clicked()
{
    emit done(ui->loadChipsCheckBox->isChecked(),
              ui->runProbeFindingCheckBox->isChecked(),
              ui->runPositionCalibrationCheckBox->isChecked(),
              m_logDirectory);
}
