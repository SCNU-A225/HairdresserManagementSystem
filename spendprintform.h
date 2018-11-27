#ifndef SPENDPRINTFORM_H
#define SPENDPRINTFORM_H

#include <QMainWindow>

namespace Ui {
class SpendPrintForm;
}

class SpendPrintForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpendPrintForm(QWidget *parent = nullptr);
    ~SpendPrintForm();

private:
    Ui::SpendPrintForm *ui;
};

#endif // SPENDPRINTFORM_H
