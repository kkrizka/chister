#include "DicedChip_ConfigureForm.h"
#include "ui_DicedChip_ConfigureForm.h"

#include <QFileDialog>

DicedChip_ConfigureForm::DicedChip_ConfigureForm(QWidget *parent)
  : QWidget(parent),
    ui(new Ui::DicedChip_ConfigureForm)
{
  ui->setupUi(this);
}

DicedChip_ConfigureForm::~DicedChip_ConfigureForm()
{
  delete ui;
}

void DicedChip_ConfigureForm::setTemplates(const QStringList& templates)
{
  for(int i=0;i<ui->templateComboBox->count();i++) ui->templateComboBox->removeItem(0);
  for(const QString& name : templates)
    ui->templateComboBox->addItem(name);
}

void DicedChip_ConfigureForm::on_logPushButton_clicked()
{
  m_logDirectory = QFileDialog::getExistingDirectory(this, tr("Select Log Directory"),
						     "",
						     QFileDialog::ShowDirsOnly);
  ui->logPushButton->setText(QString("Log: %1").arg(m_logDirectory));
}

void DicedChip_ConfigureForm::on_donePushButton_clicked()
{
  emit done(ui->templateComboBox->currentText(),
	    ui->loadChipsCheckBox->isChecked(),
	    ui->runProbeFindingCheckBox->isChecked(),
	    ui->runPositionCalibrationCheckBox->isChecked(),
	    m_logDirectory);
}
