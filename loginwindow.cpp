#include "loginwindow.h"
#include "mainwindow.h"
#include "ui_loginwindow.h"
#include <QString>
#include "databasetools.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    //获取用户输入登陆信息
    QString username = ui->userName->text();
    QString password = ui->password->text();

    //链接数据库
    DataBaseTools* tools = DataBaseTools::getDataBaseTools();
    QSqlDatabase mydb = tools->getDB();

    //查找表
    QString select_sql = QString("select username,password from user where username='%1' and password='%2'").arg(username,password);
    QSqlQuery sql_querry;
    sql_querry.exec(select_sql);

    if(sql_querry.next())//查询结果表存在
    {
        close();
        MainWindow *w = new MainWindow;
        w->show();
    }
    else
    {
            QMessageBox message(QMessageBox::NoIcon,"登陆错误","您的账号或密码错误，请重新输入！");
            message.exec();
    }
}
