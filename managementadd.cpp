#include "managementadd.h"
#include "ui_managementadd.h"

ManagementAdd::ManagementAdd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementAdd)
{
    ui->setupUi(this);
}

ManagementAdd::~ManagementAdd()
{
    delete ui;
}
