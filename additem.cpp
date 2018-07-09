#include "additem.h"
#include "ui_additem.h"

AddItem::AddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItem)
{
    ui->setupUi(this);
}

AddItem::~AddItem()
{
    delete ui;
}

void AddItem::on_buttonBox_accepted()
{
    if(ui->lineEdit->text()!="")accept();

}

void AddItem::on_buttonBox_rejected()
{
    reject();
}

QString AddItem::GetText()
{
    return ui->lineEdit->text();
}

