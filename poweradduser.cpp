#include "poweradduser.h"
#include "ui_poweradduser.h"
#include "mainwindow.h"
#include "databasetools.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasetools.h"
#include "paychangefare.h"
#include "spendaddspend.h"
#include "spendprintform.h"
#include "poweradduser.h"
#include "managementaddstaff.h"
#include "managementsearchvip.h"
#include "managementsearchstaff.h"
#include "managementaddvip.h"

#include <QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QtDebug>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>



PowerAddUser::PowerAddUser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PowerAddUser)
{
    ui->setupUi(this);
}

PowerAddUser::~PowerAddUser()
{
    delete ui;
}


void PowerAddUser::on_pushButton_enter_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    if(username=="" || password=="")
    {
        QMessageBox::warning(this,"失败","请检查输入信息！");
        return;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO user(username, password) VALUES (:username, :password)");
    query.bindValue(":username",username);	//给每个插入值标识符设定具体值
    query.bindValue(":password",password);
    if(!query.exec()){
        qDebug()<<"false";
    }
    MainWindow *mw = (MainWindow*)parentWidget();
    mw->addrowPowerTable(username);
    this->close();
}

void PowerAddUser::on_pushButton_cancel_clicked()
{
    this->close();
}

