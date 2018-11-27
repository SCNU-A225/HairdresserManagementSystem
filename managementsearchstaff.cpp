#include "managementsearchstaff.h"
#include "ui_managementsearchstaff.h"
#include "mainwindow.h"
#include <QDebug>

ManagementSearchStaff::ManagementSearchStaff(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementSearchStaff)
{
    ui->setupUi(this);
}

ManagementSearchStaff::~ManagementSearchStaff()
{
    delete ui;
}

void ManagementSearchStaff::on_btn_search_clicked()
{
    int idx = ui->comboBox->currentIndex();
    QString info = ui->textEdit_info->toPlainText();
    QString fl = "available=1 and ";
    qDebug()<<info;

    MainWindow *mw = (MainWindow*)parentWidget();
    QSqlTableModel *tabModel = mw->getTaModel();


    switch(idx)
    {
        case 0:fl+="staff_no='"+info+"'";break;
        case 1:fl+="staff_name='"+info+"'";break;
        case 2:fl+="staff_phone='"+info+"'";break;
        case 3:fl+="staff_sex='"+info+"'";break;
        case 4:fl+="staff_project='"+info+"'";break;
    }
    tabModel->setFilter(fl);
    close();
}
