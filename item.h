#ifndef ITEM_H
#define ITEM_H

#include <QWidget>

namespace Ui {
class Item;
}

class Item : public QWidget
{
    Q_OBJECT
signals:
    void itemdeleted(QString);
    void changeVal(QString,int);
public:
    explicit Item(QWidget *parent = 0);
    void setText(QString);
    void setProgress(int);
    QString getText();
    int getProgress();

    ~Item();
public slots:
    void func();
private slots:
    void on_pushButton_clicked();
private:
    Ui::Item *ui;
};

#endif // ITEM_H
