#include "spendaddspend.h"
#include "ui_spendaddspend.h"
#include "mainwindow.h"
#include "databasetools.h"
#include "QSqlQueryModel"
#include "QHeaderView"
#include <QString>
#include <QStringList>
#include <QAbstractItemView>
#include <QDebug>

SpendAddSpend::SpendAddSpend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpendAddSpend)
{
    ui->setupUi(this);
}

SpendAddSpend::~SpendAddSpend()
{
    delete ui;
}

void SpendAddSpend::showCostTable()
{
    //打开数据库
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    //查询数据库表，显示到tableview中
    model = new QSqlTableModel(this,db);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("cost");
    model->select();
    theSelection=new QItemSelectionModel(model);
    ui->tableView->setSelectionModel(theSelection); //设置选择模型

    //修改显示表头
    QStringList header;
    header<<"序号"<<"项目"<<"费用"<<"时间";
    for(int i=0; i<header.length(); i++)
        model->setHeaderData(i,Qt::Horizontal,header[i]);

    //设置模型
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();//隐藏序号
    ui->tableView->show();
}

void SpendAddSpend::on_btn_addCost_clicked()
{
    int cur_row = model->rowCount();
    model->insertRow(cur_row);
}

void SpendAddSpend::on_btn_saveCost_clicked()
{
    model->submitAll();
    MainWindow *mw = (MainWindow*)parentWidget();
    mw->openSpendTable();
    close();
}

void SpendAddSpend::on_btn_cancel_clicked()
{
    int cur_row = model->rowCount();
    model->removeRow(cur_row-1);
    model->submitAll();
}
