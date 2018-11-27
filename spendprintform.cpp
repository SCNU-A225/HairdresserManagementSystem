#include "spendprintform.h"
#include "ui_spendprintform.h"

SpendPrintForm::SpendPrintForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpendPrintForm)
{
    ui->setupUi(this);
}

SpendPrintForm::~SpendPrintForm()
{
    delete ui;
}
