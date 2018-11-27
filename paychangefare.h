#ifndef PAYCHANGEFARE_H
#define PAYCHANGEFARE_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QItemSelectionModel>
namespace Ui {
class PayChangeFare;
}

class PayChangeFare : public QMainWindow
{
    Q_OBJECT

public:
    explicit PayChangeFare(QWidget *parent = nullptr);
    ~PayChangeFare();

    void showFareTable();


private slots:
    void on_btn_addFare_clicked();

    void on_btn_deleteFare_clicked();

    void on_btn_saveFare_clicked();

private:
    Ui::PayChangeFare *ui;
    QSqlTableModel* model;
    QItemSelectionModel* theSelection;
};

#endif // PAYCHANGEFARE_H
