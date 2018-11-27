#include "managementsearch.h"
#include "ui_managementsearch.h"

ManagementSearch::ManagementSearch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementSearch)
{
    ui->setupUi(this);
}

ManagementSearch::~ManagementSearch()
{
    delete ui;
}
