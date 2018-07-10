//This is the Dialog to add your modules information to be preprocessed into grades
#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QList<QString> grades;
    grades.append("Choose your grade");
    grades.append("A+");
    grades.append("A");
    grades.append("A-");
    grades.append("B+");
    grades.append("B");
    grades.append("B-");
    grades.append("C+");
    grades.append("C");
    grades.append("D+");
    grades.append("D");
    grades.append("F");
    ui->comboBox->addItems(grades);
}

QString Dialog::Module()
{
    return ui->lineEdit->text();
}

int Dialog::Creditunits()
{
    return ui->spinBox->value();
}

QString Dialog::Grade()
{
    return ui->comboBox->currentText();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    if(ui->comboBox->currentText()=="Choose your grade"||Grade()==0||Module()==NULL){
        QMessageBox::warning(this,"Incomplete","Please fill in the details correctly");
    }
    else accept();
}

void Dialog::on_buttonBox_rejected()
{
    reject();
}
