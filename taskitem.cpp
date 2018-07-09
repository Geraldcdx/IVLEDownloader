#include "taskitem.h"
#include "ui_taskitem.h"

taskItem::taskItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::taskItem)
{
    ui->setupUi(this);
}

taskItem::~taskItem()
{
    delete ui;
}
