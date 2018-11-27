#include "managementaddstaff.h"
#include "ui_managementaddstaff.h"
#include "databasetools.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

ManagementAddStaff::ManagementAddStaff(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagementAddStaff)
{
    ui->setupUi(this);
    DataBaseTools *tool = DataBaseTools::getDataBaseTools();
    //获取字段下拉框
    QStringList strList = tool->getFieldValues("project","pro_name");
    ui->comboBox_project->addItems(strList);

}

ManagementAddStaff::~ManagementAddStaff()
{
    delete ui;
}

void ManagementAddStaff::on_btn_submit_clicked()
{
    QSqlQuery sqlQuery;
    QString sql;
    QString name,phone,sex,no,salary,project;
    //获取表单
    name = ui->lineEdit_name->text();
    no = ui->lineEdit_no->text();
    phone = ui->lineEdit_phone->text();
    salary = ui->lineEdit_salary->text();
    sex = ui->comboBox_sex->currentText();
    project = ui->comboBox_project->currentText();
    if(name==""||no==""||salary==""||sex==""||project=="")
    {
        QMessageBox::warning(this,"添加错误","请检查是否输入了信息！");
        return;
    }

    //插入数据
    sql = "insert into staff (staff_no,staff_name,staff_sex,staff_salary,staff_project,staff_phone,available) "
          "values ('%1','%2','%3','%4','%5','%6',1)";
    sql = sql.arg(no,name,sex,salary,project,phone);
    if(!sqlQuery.exec(sql))
    {
        QMessageBox::warning(this,"添加错误",sqlQuery.lastError().text());
        return;
    }
    bool ok = QMessageBox::information(this,"添加成功","添加员工"+name+"成功！",QMessageBox::Ok);
    if(ok)
    {
        close();
    }
}

void ManagementAddStaff::closeEvent(QCloseEvent *event)
{
    MainWindow *mw = (MainWindow*)parentWidget();
    mw->openTable(WINDOW_TABLE::MANAGE_STAFF);
    event->accept();
}
