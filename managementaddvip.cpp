#include "managementaddvip.h"
#include "ui_managementaddvip.h"
#include "databasetools.h"
#include "mainwindow.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QtDebug>
#include <QSqlQuery>

ManagementAddVip::ManagementAddVip(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementAddVip)
{
    ui->setupUi(this);
}

ManagementAddVip::~ManagementAddVip()
{
    delete ui;
}

void ManagementAddVip::on_submit_clicked()
{
    DataBaseTools* tools = DataBaseTools::getDataBaseTools();
    QSqlDatabase mydb = tools->getDB();

    QSqlQuery sqlQuery;
    QString sql;
    QString cardNumber,name,sex,callNumber,cost;
    cardNumber=ui->cardNumber->text();
    name=ui->name->text();
    sex=ui->sex->currentText();
    callNumber=ui->callNumber->text();
    cost=ui->cost->text();

    if(name==""||cardNumber==""||callNumber==""||sex==""||cost=="")
    {
        QMessageBox::warning(this,"添加错误","请检查是否输入了信息！");
        return;
    }

    //插入数据
    sql="insert into vip(vip_no,vip_name,vip_sex,vip_phone,vip_consumption,available)""values('%1','%2','%3','%4',%5,1)";
    sql=sql.arg(cardNumber,name,sex,callNumber).arg(cost);

    if(!sqlQuery.exec(sql))
    {
        QMessageBox::warning(this,"添加错误","数据库错误！code:003");
        qDebug()<<"数据库错误！code:003";
        return;
    }
    bool ok = QMessageBox::information(this,"添加成功","添加会员"+name+"成功！",QMessageBox::Ok);
    if(ok)
    {
        close();
    }
}

void ManagementAddVip::closeEvent(QCloseEvent *event)
{
    MainWindow *mw = (MainWindow*)parentWidget();
    mw->openTable(WINDOW_TABLE::MANAGE_VIP);
    event->accept();
}
