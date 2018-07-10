//Form is used to create a new module tab for announcements
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}
void Form ::settingofText(QString text)
{
    ui->textBrowser->setText(text);

}

