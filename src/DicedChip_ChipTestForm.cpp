#include "DicedChip_ChipTestForm.h"
#include "ui_DicedChip_ChipTestForm.h"

#include <QMessageBox>
#include <QDebug>

DicedChip_ChipTestForm::DicedChip_ChipTestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_ChipTestForm)
{
  ui->setupUi(this);

  ui->confirmPushButton->setEnabled(true );
  ui->skipPushButton   ->setEnabled(true );
  ui->nextPushButton   ->setEnabled(false);
}

DicedChip_ChipTestForm::~DicedChip_ChipTestForm()
{
  delete ui;
}

void DicedChip_ChipTestForm::setupSlots(uint nX, uint nY)
{
  uint cnt=0;
  for(uint iY=0;iY<nY;iY++)    
    {
      for(uint iX=0;iX<nX;iX++)
	{
	  QPushButton *button=new QPushButton(this);
	  button->setText(QString::number(cnt));
	  button->resize(20,20);
	  button->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	  connect(button,&QPushButton::clicked,this,&DicedChip_ChipTestForm::on_slotPushButton_clicked);
	  ui->slotsGridLayout->addWidget(button,iY,iX);
	  m_slotChecks[button]=new DicedChipSlot(iX,iY,this);
	  cnt++;
        }
    }
}

void DicedChip_ChipTestForm::setValidSlots(const QList<DicedChipSlot*>& validSlots)
{
  for(QPushButton *check : m_slotChecks.keys())
    {
      check->setStyleSheet("");
      for(DicedChipSlot* slot : validSlots)
	{
	  if(m_slotChecks[check]->slot()==slot->slot()) // Found position match
	    {
	      QObject::disconnect(m_slotChecks[check],0,this,0);
	      m_slotChecks[check]=slot;
	      if(!slot->m_name.isEmpty()) check->setText(slot->m_name);
	      check->setStyleSheet("font-weight: bold");
	      break;
	    }
	}
    }
}

void DicedChip_ChipTestForm::on_slotPushButton_clicked()
{
  QPushButton *sourceSlot=dynamic_cast<QPushButton*>(sender());
  emit findChip(m_slotChecks[sourceSlot]);
}

void DicedChip_ChipTestForm::on_redoPushButton_clicked()
{
  emit alignChip();
}

void DicedChip_ChipTestForm::on_confirmPushButton_clicked()
{
  emit confirmChip();
}

void DicedChip_ChipTestForm::updateChipSlot(DicedChipSlot* slot)
{
  ui->chipFoundLabel->setText("Finding chip...");
  ui->chipSlotLabel->setText(QString("Active Slot: %1 (%2,%3)").arg(slot->slot().second*6+slot->slot().first).arg(slot->slot().first).arg(slot->slot().second));
}

void DicedChip_ChipTestForm::updateChipTestScore(float chipOffsetScore)
{
  ui->chipFoundLabel->setText(QString("Chip alignment score: %1").arg(chipOffsetScore));
}

void DicedChip_ChipTestForm::updateChipSlotStyle(const DicedChipSlot* slot)
{
  QString style="";
  if(slot->m_status!=DicedChipSlot::Empty) style+="font-weight: bold; ";

  if(slot->m_status==DicedChipSlot::Pass) style+="color: #00FF00; ";
  else if(slot->m_status==DicedChipSlot::Fail) style+="color: #FF0000; ";
  else if(slot->m_status==DicedChipSlot::Misaligned) style+="color: #BCBCBC; ";
  else if(!slot->position().isNull()) style+="color: #00BFFF; ";

  for(QPushButton *check : m_slotChecks.keys())
    {
      if(m_slotChecks[check]==slot)
	{
	  check->setStyleSheet(style);
	  break;
	}
    }
}

void DicedChip_ChipTestForm::on_skipPushButton_clicked()
{
  emit skipChip();
}

void DicedChip_ChipTestForm::on_nextPushButton_clicked()
{
  emit nextChip();
}

void DicedChip_ChipTestForm::on_testPushButton_clicked()
{
  emit testChip();
}

void DicedChip_ChipTestForm::prepareAlignChips()
{
  ui->confirmPushButton->setEnabled(true );
  ui->skipPushButton   ->setEnabled(true );
  ui->nextPushButton   ->setEnabled(false);
}


void DicedChip_ChipTestForm::prepareTestChips()
{
  int ret = QMessageBox::question(this, "Start Chip Testing", "Ready to start chip testing?",
				  QMessageBox::Yes|QMessageBox::No);
  if(ret == QMessageBox::Yes)
    {
      ui->nextPushButton   ->setEnabled(true );
      ui->confirmPushButton->setEnabled(false);
      ui->skipPushButton   ->setEnabled(false);
      emit nextChip();
    }
}
