#ifndef SPENDADDSPEND_H
#define SPENDADDSPEND_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QItemSelectionModel>

namespace Ui {
class SpendAddSpend;
}

class SpendAddSpend : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpendAddSpend(QWidget *parent = nullptr);
    ~SpendAddSpend();
    void showCostTable();

private slots:

    void on_btn_addCost_clicked();

    void on_btn_saveCost_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::SpendAddSpend *ui;
    QSqlTableModel* model;
    QItemSelectionModel* theSelection;
};

#endif // SPENDADDSPEND_H
