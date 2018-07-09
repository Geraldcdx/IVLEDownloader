#ifndef ADDITEM_H
#define ADDITEM_H

#include <QDialog>

namespace Ui {
class AddItem;
}

class AddItem : public QDialog
{
    Q_OBJECT

public:
    explicit AddItem(QWidget *parent = 0);
    QString GetText();
    ~AddItem();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddItem *ui;
};

#endif // ADDITEM_H
