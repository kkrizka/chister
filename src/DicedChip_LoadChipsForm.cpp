#include "DicedChip_LoadChipsForm.h"
#include "ui_DicedChip_LoadChipsForm.h"

#include <QInputDialog>
#include <QDebug>

DicedChip_LoadChipsForm::DicedChip_LoadChipsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_LoadChipsForm)
{
  ui->setupUi(this);
}

DicedChip_LoadChipsForm::~DicedChip_LoadChipsForm()
{
  delete ui;
}

void DicedChip_LoadChipsForm::setupSlots(uint nX, uint nY)
{
  uint cnt=0;
  for(uint iY=0;iY<nY;iY++)
    {
      for(uint iX=0;iX<nX;iX++)
	{
	  QPushButton *button=new QPushButton(this);
	  button->setText(QString::number(cnt));
	  button->setCheckable(true);
	  connect(button, &QPushButton::clicked, this, &DicedChip_LoadChipsForm::on_slotPushButton_clicked);
	  button->resize(20,20);
	  button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	  ui->slotsGridLayout->addWidget(button,iY,iX);
	  m_slotChecks[button]=new DicedChipSlot(iX,iY);
	  cnt++;
        }
    }
}

void DicedChip_LoadChipsForm::on_slotPushButton_clicked(bool checked)
{
QPushButton *button=dynamic_cast<QPushButton*>(QObject::sender());
  if(checked)
    {
      // Try to guess chip name based on last one
      QString chipNameGuess="";
      if(!m_lastChipName.isNull())
	{
	  static const QRegularExpression re_name("([a-zA-Z]+)([0-9]+)");
	  QRegularExpressionMatch match=re_name.match(m_lastChipName);
	  if(match.hasMatch())
	    {
	      QString name_str=match.captured(1);
	      QString name_num=match.captured(2);
	      uint name_num_uint=name_num.toUInt();
	      name_num_uint++;
	      chipNameGuess=QString("%1%2").arg(name_str).arg(name_num_uint,name_num.length(),10,QChar('0'));
	    }
	  else
	    chipNameGuess=m_lastChipName;
	}

      // Ask for chip name
      bool ok;
      QString chipName = QInputDialog::getText(this, tr("Chip Name"),
					       tr("Chip Name:"), QLineEdit::Normal,
					       chipNameGuess, &ok);
      if (!ok || chipName.isEmpty())
	chipName=button->text();

      m_lastChipName=chipName;
      m_slotChecks[button]->m_name=chipName;
      button->setText(chipName);
    }
}

void DicedChip_LoadChipsForm::on_donePushButton_clicked()
{
  QList<DicedChipSlot*> validslots;
  for(QPushButton *check : m_slotChecks.keys())
    {
      if(check->isChecked()) 
	{
	  m_slotChecks[check]->m_status=DicedChipSlot::Untested;
	  validslots.append(m_slotChecks[check]);
	}
    }
  emit done(validslots);
}

void DicedChip_LoadChipsForm::on_selectPushButton_clicked()
{
  for(QPushButton *check : m_slotChecks.keys())
    {
      check->setChecked(true);
    }
}

void DicedChip_LoadChipsForm::on_deselectPushButton_clicked()
{
  for(QPushButton *check : m_slotChecks.keys())
    {
      check->setChecked(false);
    }
}
