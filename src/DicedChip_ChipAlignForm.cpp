#include "DicedChip_ChipAlignForm.h"
#include "ui_DicedChip_ChipAlignForm.h"

#include <QMessageBox>
#include <QDebug>

DicedChip_ChipAlignForm::DicedChip_ChipAlignForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicedChip_ChipAlignForm)
{
    ui->setupUi(this);
}

DicedChip_ChipAlignForm::~DicedChip_ChipAlignForm()
{
    delete ui;
}

void DicedChip_ChipAlignForm::setupSlots(uint nX, uint nY)
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
	  connect(button,&QPushButton::clicked,this,&DicedChip_ChipAlignForm::on_slotPushButton_clicked);
	  ui->slotsGridLayout->addWidget(button,iY,iX);
	  m_slotChecks[button]=new DicedChipSlot(iX,iY,this);
	  cnt++;
        }
    }
}

void DicedChip_ChipAlignForm::setValidSlots(const QList<DicedChipSlot*>& validSlots)
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
	      check->setStyleSheet("font-weight: bold");
	      break;
	    }
	}
    }
}

void DicedChip_ChipAlignForm::on_slotPushButton_clicked()
{
  QPushButton *sourceSlot=dynamic_cast<QPushButton*>(sender());
  emit findChip(m_slotChecks[sourceSlot]);
}

void DicedChip_ChipAlignForm::on_redoPushButton_clicked()
{
  emit alignChip();
}

void DicedChip_ChipAlignForm::on_confirmPushButton_clicked()
{
  emit confirmChip();
}

void DicedChip_ChipAlignForm::updateChipSlot(DicedChipSlot* slot)
{
  ui->chipFoundLabel->setText("Finding chip...");
  ui->chipSlotLabel->setText(QString("Active Slot: %1 (%2,%3)").arg(slot->slot().first*2+slot->slot().second).arg(slot->slot().first).arg(slot->slot().second));
}

void DicedChip_ChipAlignForm::updateChipAlignScore(float chipOffsetScore)
{
  ui->chipFoundLabel->setText(QString("Chip alignment score: %1").arg(chipOffsetScore));
}

void DicedChip_ChipAlignForm::updateChipSlotStyle(const DicedChipSlot* slot)
{
  QString style="";
  qInfo() << (slot->m_status==DicedChipSlot::Empty);
  if(slot->m_status!=DicedChipSlot::Empty) style+="font-weight: bold; ";
  if(!slot->position().isNull()) style+="color: #00BFFF; ";

  for(QPushButton *check : m_slotChecks.keys())
    {
      if(m_slotChecks[check]==slot)
	{
	  check->setStyleSheet(style);
	  break;
	}
    }
}

void DicedChip_ChipAlignForm::on_skipPushButton_clicked()
{
  emit nextChip();
}



// void DicedChip_ChipAlignForm::updateChipStatus(bool result, const QString& testLog)
// {
//   if(result)
//     {
//       QMessageBox msgBox;
//       msgBox.setText("Chip test succeeded.");
//       msgBox.exec();
//       emit nextChip();
//     }
//   else
//     {
//       QMessageBox msgBox;
//       msgBox.setText("Chip test failed.");
//       msgBox.setInformativeText("Do you want to procceed?");
//       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//       msgBox.setDefaultButton(QMessageBox::No);
//       msgBox.setDetailedText(testLog);
//       int ret=msgBox.exec();
//       switch(ret)
//         {
//         case QMessageBox::Yes:
// 	  emit nextChip();
// 	  break;
//         case QMessageBox::No:
//         default:
// 	  break;
//         }
//     }
// }

