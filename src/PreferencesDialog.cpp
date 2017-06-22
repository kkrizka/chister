#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferencesDialog)
{
  ui->setupUi(this);

  connect(ui->preferencesListWidget, &QListWidget::currentItemChanged, this, &PreferencesDialog::changeForm);
}

PreferencesDialog::~PreferencesDialog()
{
  delete ui;
}

void PreferencesDialog::on_okPushButton_clicked()
{ close(); }

void PreferencesDialog::on_cancelPushButton_clicked()
{ close(); }

void PreferencesDialog::addForm(const QString& title, QWidget *widget)
{
  QListWidgetItem *listButton = new QListWidgetItem(ui->preferencesListWidget);
  listButton->setText(title);
  listButton->setTextAlignment(Qt::AlignHCenter);
  listButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  ui->mainStackedWidget->addWidget(widget);
}

void PreferencesDialog::changeForm(QListWidgetItem *current, QListWidgetItem *previous)
{
  if(!current) current=previous;

  ui->mainStackedWidget->setCurrentIndex(ui->preferencesListWidget->row(current));
}
