#include "paychangefare.h"
#include "ui_paychangefare.h"
#include "databasetools.h"
#include "QSqlQueryModel"
#include "QHeaderView"
#include <QString>
#include <QStringList>
#include <QAbstractItemView>
#include <QDebug>


PayChangeFare::PayChangeFare(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PayChangeFare)
{
    ui->setupUi(this);
}

PayChangeFare::~PayChangeFare()
{
    delete ui;
}

void PayChangeFare::showFareTable()
{
    //打开数据库
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    //查询数据库表，显示到tableview中
    model = new QSqlTableModel(this,db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("project");
    model->select();
    theSelection=new QItemSelectionModel(model);
    ui->tw_fareTable->setSelectionModel(theSelection); //设置选择模型

    //修改显示表头
    QStringList header;
    header<<"项目编号"<<"项目名称"<<"原价"<<"折扣百分比";
    for(int i=0; i<header.length(); i++)
        model->setHeaderData(i,Qt::Horizontal,header[i]);

    //设置模型
    ui->tw_fareTable->setModel(model);
    ui->tw_fareTable->verticalHeader()->hide();//隐藏序号
    ui->tw_fareTable->show();
}

void PayChangeFare::on_btn_addFare_clicked()
{
    int cur_row = model->rowCount();
    model->insertRow(cur_row);
}

void PayChangeFare::on_btn_deleteFare_clicked()
{
    int cur_row = model->rowCount();
    model->removeRow(cur_row-1);
    model->submitAll();
}

void PayChangeFare::on_btn_saveFare_clicked()
{
    model->submitAll();
    close();
}
