//This is the dialog box thata pops up to add values to create an Item for the To-do-list
#include "additem.h"
#include "ui_additem.h"
#include <QMessageBox>

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
    if(ui->lineEdit->text()=="")
        QMessageBox::warning(this,"Incomplete","Please fill in the details correctly");
    else accept();

}

void AddItem::on_buttonBox_rejected()
{
    reject();
}

QString AddItem::GetText()
{
    return ui->lineEdit->text();
}

