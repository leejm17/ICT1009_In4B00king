#ifndef ECONOMYHALL_H
#define ECONOMYHALL_H

#include <QDialog>
#include "mydb.h"

namespace Ui {
class EconomyHall;
}

class EconomyHall : public QDialog
{
    Q_OBJECT

public:
    explicit EconomyHall(QWidget *parent = nullptr);
    ~EconomyHall();

signals:
    void showSeatSelection(int);
    void closeAll();

private slots:
    void on_book_clicked();
    void updateSeats(QString, QString, int);

private:
    Ui::EconomyHall *ui;
    void closeEvent(QCloseEvent *event);
    int show_ID;
};

#endif // ECONOMYHALL_H
