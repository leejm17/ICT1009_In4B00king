#ifndef ECONOMYHALL_H
#define ECONOMYHALL_H

#include <QDialog>
#include "seatselection.h"

namespace Ui {
class EconomyHall;
}

class EconomyHall : public QDialog
{
    Q_OBJECT

public:
    explicit EconomyHall(QWidget *parent = nullptr);
    ~EconomyHall();
    void updateSeats();

private slots:
    void on_book_clicked();

private:
    Ui::EconomyHall *ui;
    SeatSelection *seatSelection;
};

#endif // ECONOMYHALL_H