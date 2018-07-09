#ifndef TASKITEM_H
#define TASKITEM_H

#include <QWidget>

namespace Ui {
class taskItem;
}

class taskItem : public QWidget
{
    Q_OBJECT

public:
    explicit taskItem(QWidget *parent = 0);
    ~taskItem();

private:
    Ui::taskItem *ui;
};

#endif // TASKITEM_H
