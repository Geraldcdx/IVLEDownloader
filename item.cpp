//This folder file is used to create the Item widget for the to-do-list
#include "item.h"
#include "ui_item.h"
#include "mainwindow.h"
Item::Item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Item)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMaximum(100);
    connect(ui->horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(func()));
}

void Item::func()
{
    emit changeVal(getText(),getProgress());
}
Item::~Item()
{
    delete ui;
}

void Item::on_pushButton_clicked()
{
    if(ui->progressBar->value()==100){
        emit itemdeleted(getText());
        delete this;
    }
}

void Item::setText(QString str)
{
    ui->label->setText(str);
}
void Item::setProgress(int num)
{
    ui->progressBar->setValue(num);
    ui->horizontalSlider->setValue(num);
}

QString Item:: getText()
{
    return ui->label->text();
}
int Item::getProgress()
{
    return ui->progressBar->value();
}
