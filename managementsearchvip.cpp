#include "managementsearchvip.h"
#include "ui_managementsearchvip.h"
#include "mainwindow.h"
#include <QSqlTableModel>

ManagementSearchVip::ManagementSearchVip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementSearchVip)
{
    ui->setupUi(this);
}

ManagementSearchVip::~ManagementSearchVip()
{
    delete ui;
}

void ManagementSearchVip::on_btn_search_clicked()
{
    int idx = ui->comboBox->currentIndex();
    QString info = ui->textEdit_info->toPlainText();
    QString fl = "available=1 and ";

    MainWindow *mw = (MainWindow*)parentWidget();
    QSqlTableModel *tabModel = mw->getTaModel();


    switch(idx)
    {
        case 0:fl+="vip_no='"+info+"'";break;
        case 1:fl+="vip_name='"+info+"'";break;
        case 2:fl+="vip_phone='"+info+"'";break;
        case 3:fl+="vip_sex='"+info+"'";break;
    }
    tabModel->setFilter(fl);
    close();
}
