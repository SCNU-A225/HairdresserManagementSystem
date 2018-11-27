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
#include <QMessageBox>
#include <QLineEdit>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openTable(MANAGE_STAFF);
    openHistoryTable(MANAGE_STAFF);
    //获取字段下拉框
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QStringList strList = dbTools->getFieldValues("project","pro_name");
    ui->comboBox_mStaffProject->addItems(strList);
}

void MainWindow::openTable(WINDOW_TABLE wt)
{
    //锁定信息
    int index = ui->tabWidget->currentIndex();
    setInfoLock(WINDOW_TABLE(index),true);

    //连接数据库，获取db对象
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    tabModel=new QSqlTableModel(this,db);//数据表
    QTableView * tableView;
    QString tableName,sortField;
    QStringList filedList,fieldNameList,hiddenList;
    switch(wt)
    {
    case MANAGE_STAFF:
        tableView = ui->tableView_mStaff;
        tableName = "staff";
        sortField = "staff_no";
        filedList<<"staff_no"<<"staff_name"<<"staff_sex";
        fieldNameList<<"编号"<<"姓名"<<"性别";
        hiddenList<<"staff_phone"<<"staff_photo_path"<<"staff_salary"<<"staff_project"<<"available";
        break;
    case MANAGE_VIP:
        tableView = ui->tableView_mVip;
        tableName = "vip";
        sortField = "vip_no";
        filedList<<"vip_no"<<"vip_name"<<"vip_sex";
        fieldNameList<<"卡号"<<"姓名"<<"性别";
        hiddenList<<"vip_phone"<<"vip_photo_path"<<"vip_consumption"<<"available";
        break;
    case STATISTIC_STAFF:
        tableView = ui->tableView_sStaff;
        tableName = "staff";
        sortField = "staff_salary";
        filedList<<"staff_no"<<"staff_name"<<"staff_salary";
        fieldNameList<<"编号"<<"姓名"<<"工资";
        hiddenList<<"staff_phone"<<"staff_photo_path"<<"staff_sex"<<"staff_project"<<"available";
        break;
    case STATISTIC_VIP:
        tableView = ui->tableView_sVip;
        tableName = "vip";
        sortField = "vip_consumption";
        filedList<<"vip_no"<<"vip_name"<<"vip_consumption";
        fieldNameList<<"卡号"<<"姓名"<<"消费";
        hiddenList<<"vip_phone"<<"vip_photo_path"<<"vip_sex"<<"available";
        break;


    }
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行
    tableView->setSelectionMode(QAbstractItemView::SingleSelection); //设置只能选择一行，不能多行选中
    //tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);   //设置每行内容不可更改

    tabModel->setTable(tableName); //设置数据表
    tabModel->setFilter(tr("available=%1").arg(1));
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
    if(wt==WINDOW_TABLE::MANAGE_STAFF||wt==WINDOW_TABLE::MANAGE_VIP)
        tabModel->setSort(tabModel->fieldIndex(sortField),Qt::AscendingOrder); //升序排序
    else
        tabModel->setSort(tabModel->fieldIndex(sortField),Qt::DescendingOrder);
    //判断数据库能否正常使用
    if (!(tabModel->select()))//查询数据
    {
       QMessageBox::critical(this, "错误信息",
              "打开数据表错误,错误信息\n"+tabModel->lastError().text(),
                 QMessageBox::Ok,QMessageBox::NoButton);
       return;
    }
    //字段显示名
    for(int i=0;i<filedList.length();i++)
        tabModel->setHeaderData(tabModel->fieldIndex(filedList[i]),Qt::Horizontal,fieldNameList[i]);
    //设置数据模型
    tableView->setModel(tabModel);
    //隐藏字段
    for(int i=0;i<hiddenList.length();i++)
        tableView->setColumnHidden(tabModel->fieldIndex(hiddenList[i]),true);

    theSelection=new QItemSelectionModel(tabModel);//关联选择模型
    connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex))); //选择行变化时
    tableView->setSelectionModel(theSelection); //设置选择模型

    dataMapper = new QDataWidgetMapper();//新建映射对象
    dataMapper->setModel(tabModel);//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);//设置映射保存方式
    switch(wt)
    {
    case MANAGE_STAFF:
        dataMapper->addMapping(ui->lineEdit_mStaffName,tabModel->fieldIndex("staff_name"));
        dataMapper->addMapping(ui->comboBox_mStaffSex,tabModel->fieldIndex("staff_sex"));
        dataMapper->addMapping(ui->lineEdit_mStaffNo,tabModel->fieldIndex("staff_no"));
        dataMapper->addMapping(ui->lineEdit_mStaffPhone,tabModel->fieldIndex("staff_phone"));
        dataMapper->addMapping(ui->lineEdit_mStaffSalary,tabModel->fieldIndex("staff_salary"));
        dataMapper->addMapping(ui->comboBox_mStaffProject,tabModel->fieldIndex("staff_project"));
        break;
    case MANAGE_VIP:
        dataMapper->addMapping(ui->lineEdit_mVipName,tabModel->fieldIndex("vip_name"));
        dataMapper->addMapping(ui->comboBox_mVipSex,tabModel->fieldIndex("vip_sex"));
        dataMapper->addMapping(ui->lineEdit_mVipNo,tabModel->fieldIndex("vip_no"));
        dataMapper->addMapping(ui->lineEdit_mVipPhone,tabModel->fieldIndex("vip_phone"));
        dataMapper->addMapping(ui->lineEdit_mVipConsumption,tabModel->fieldIndex("vip_consumption"));
        break;
    case STATISTIC_STAFF:
        dataMapper->addMapping(ui->lineEdit_sStaffName,tabModel->fieldIndex("staff_name"));
        dataMapper->addMapping(ui->lineEdit_sStaffSex,tabModel->fieldIndex("staff_sex"));
        dataMapper->addMapping(ui->lineEdit_sStaffNo,tabModel->fieldIndex("staff_no"));
        dataMapper->addMapping(ui->lineEdit_sStaffPhone,tabModel->fieldIndex("staff_phone"));
        dataMapper->addMapping(ui->lineEdit_sStaffSalary,tabModel->fieldIndex("staff_salary"));
        dataMapper->addMapping(ui->comboBox_sStaffProject,tabModel->fieldIndex("staff_project"));
        break;
    case STATISTIC_VIP:
        dataMapper->addMapping(ui->lineEdit_sVipName,tabModel->fieldIndex("vip_name"));
        dataMapper->addMapping(ui->lineEdit_sVipSex,tabModel->fieldIndex("vip_sex"));
        dataMapper->addMapping(ui->lineEdit_sVipNo,tabModel->fieldIndex("vip_no"));
        dataMapper->addMapping(ui->lineEdit_sVipPhone,tabModel->fieldIndex("vip_phone"));
        dataMapper->addMapping(ui->lineEdit_sVipConsumption,tabModel->fieldIndex("vip_consumption"));
        break;
    }

//界面组件与tabModel的具体字段之间的联系

//    dataMapper->addMapping(ui->dbPhoto,tabModel->fieldIndex("Photo")); //图片无法直接映射

    dataMapper->toFirst();//移动到首记录

}

void MainWindow::openHistoryTable(WINDOW_TABLE wt)
{
    QString no = ((QLineEdit*)dataMapper->mappedWidgetAt(0))->text();//获取顾客或员工编号
    QSqlQueryModel *hisSqlModel;
    QString sql;
    switch(wt)
    {
    case WINDOW_TABLE::MANAGE_STAFF:
        hisSqlModel = new QSqlQueryModel(ui->tableView_mStaffHistory);
        sql = QString("select history_time,history_item_name,vip_name,history_cost from history,vip "
                              "where history.history_vip_no=vip.vip_no and history.history_staff_no='%1'").arg(no);
        hisSqlModel->setQuery(sql);
        hisSqlModel->setHeaderData(0, Qt::Horizontal, tr("时间"));
        hisSqlModel->setHeaderData(1, Qt::Horizontal, tr("项目名"));
        hisSqlModel->setHeaderData(2, Qt::Horizontal, tr("顾客"));
        hisSqlModel->setHeaderData(3, Qt::Horizontal, tr("金额"));
        ui->tableView_mStaffHistory->setModel(hisSqlModel);
        break;
    case WINDOW_TABLE::MANAGE_VIP:
        hisSqlModel = new QSqlQueryModel(ui->tableView_mVipHistory);
        sql = QString("select history_time,history_item_name,staff_name,history_cost from history,staff "
                              "where history.history_staff_no=staff.staff_no and history.history_vip_no='%1'").arg(no);
        hisSqlModel->setQuery(sql);
        hisSqlModel->setHeaderData(0, Qt::Horizontal, tr("时间"));
        hisSqlModel->setHeaderData(1, Qt::Horizontal, tr("项目名"));
        hisSqlModel->setHeaderData(2, Qt::Horizontal, tr("服务员"));
        hisSqlModel->setHeaderData(3, Qt::Horizontal, tr("金额"));
        ui->tableView_mVipHistory->setModel(hisSqlModel);
    case WINDOW_TABLE::STATISTIC_STAFF:
        hisSqlModel = new QSqlQueryModel(ui->tableView_sStaffHistory);
        sql = QString("select history_time,history_item_name,vip_name,history_cost from history,vip "
                              "where history.history_vip_no=vip.vip_no and history.history_staff_no='%1'").arg(no);
        hisSqlModel->setQuery(sql);
        hisSqlModel->setHeaderData(0, Qt::Horizontal, tr("时间"));
        hisSqlModel->setHeaderData(1, Qt::Horizontal, tr("项目名"));
        hisSqlModel->setHeaderData(2, Qt::Horizontal, tr("顾客"));
        hisSqlModel->setHeaderData(3, Qt::Horizontal, tr("金额"));
        ui->tableView_sStaffHistory->setModel(hisSqlModel);
        break;
    case WINDOW_TABLE::STATISTIC_VIP:
        hisSqlModel = new QSqlQueryModel(ui->tableView_sVipHistory);
        sql = QString("select history_time,history_item_name,staff_name,history_cost from history,staff "
                              "where history.history_staff_no=staff.staff_no and history.history_vip_no='%1'").arg(no);
        hisSqlModel->setQuery(sql);
        hisSqlModel->setHeaderData(0, Qt::Horizontal, tr("时间"));
        hisSqlModel->setHeaderData(1, Qt::Horizontal, tr("项目名"));
        hisSqlModel->setHeaderData(2, Qt::Horizontal, tr("服务员"));
        hisSqlModel->setHeaderData(3, Qt::Horizontal, tr("金额"));
        ui->tableView_sVipHistory->setModel(hisSqlModel);
        break;
    }

}

void MainWindow::switchPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_management_triggered()
{
    switchPage(0);
}

void MainWindow::on_pay_triggered()
{
    switchPage(1);
    //初始化选择项目下拉列表
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();
    QSqlQuery pro_query;
    QString select="select pro_name from project";
    pro_query.exec(select);
    ui->cb_payPro->clear();
    while (pro_query.next())
    {
        ui->cb_payPro->addItem(pro_query.value(0).toString());
    }
    ui->cb_payPro->setCurrentIndex(-1);
}

void MainWindow::on_spend_triggered()
{
    switchPage(2);
    openSpendTable();
}

void MainWindow::on_statistics_triggered()
{
    switchPage(3);
    openTable(WINDOW_TABLE::STATISTIC_STAFF);
    openHistoryTable(WINDOW_TABLE::STATISTIC_STAFF);
}

void MainWindow::on_power_triggered()
{
    switchPage(4);
    openPowerTable();//权限页面函数调用
}

//修改资费标准
void MainWindow::on_btn_changeFare_clicked()
{
    PayChangeFare* w = new PayChangeFare(this);
    w->showFareTable();
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}


void MainWindow::on_btn_addSpend_clicked()
{
    SpendAddSpend* w = new SpendAddSpend(this);
    w->showCostTable();
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_printForm_clicked()
{
    SpendPrintForm* w = new SpendPrintForm(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_addUser_clicked()
{
    PowerAddUser* w = new PowerAddUser(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_searchStaff_clicked()
{
    ManagementSearchStaff* w = new ManagementSearchStaff(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_addStaff_clicked()
{
    ManagementAddStaff* w = new ManagementAddStaff(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_searchVip_clicked()
{
    ManagementSearchVip* w = new ManagementSearchVip(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_addVip_clicked()
{
    ManagementAddVip* w = new ManagementAddVip(this);
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_btn_addVip2_clicked()
{
    ManagementAddVip* w = new ManagementAddVip;
    w->setWindowModality(Qt::WindowModal);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    openTable(WINDOW_TABLE(index));
}

void MainWindow::on_tabWidget_2_currentChanged(int index)
{
    openTable(WINDOW_TABLE(index+2));
}

void MainWindow::setInfoLock(WINDOW_TABLE wt,bool lock)
{
    switch(wt)
    {
    case WINDOW_TABLE::MANAGE_STAFF:
        ui->lineEdit_mStaffName->setReadOnly(lock);
        ui->lineEdit_mStaffNo->setReadOnly(lock);
        ui->lineEdit_mStaffSalary->setReadOnly(lock);
        ui->comboBox_mStaffProject->setDisabled(lock);
        ui->comboBox_mStaffSex->setDisabled(lock);
        break;
    case WINDOW_TABLE::MANAGE_VIP:
        ui->lineEdit_mVipName->setReadOnly(lock);
        ui->lineEdit_mVipNo->setReadOnly(lock);
        ui->lineEdit_mVipPhone->setReadOnly(lock);
        ui->lineEdit_mVipConsumption->setReadOnly(lock);
        ui->comboBox_mVipSex->setDisabled(lock);
        break;
    }
    infoLock = lock?true:false;
}

void MainWindow::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    if(ui->stackedWidget->currentIndex()==0)
    {
        int index = ui->tabWidget->currentIndex();//获取tab下标
        dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号
        openHistoryTable(WINDOW_TABLE(index));
        setInfoLock(WINDOW_TABLE(index),true);//锁定信息
        ui->btn_editStaffInfo->setText(tr("修改"));
        ui->btn_editVipInfo->setText(tr("修改"));
        tabModel->revertAll();
    }
    else
    {
        int index = ui->tabWidget_2->currentIndex();//获取tab下标
        dataMapper->setCurrentIndex(current.row()); //更新数据映射的行号
        openHistoryTable(WINDOW_TABLE(index+2));
    }

}

void MainWindow::on_btn_editStaffInfo_clicked()
{
    if(infoLock)
    {
        //设置为可以修改
        setInfoLock(WINDOW_TABLE::MANAGE_STAFF,false);
        ui->btn_editStaffInfo->setText(tr("保存"));
    }
    else
    {

        if((!dataMapper->submit())||(!tabModel->submitAll()))
        {
          tabModel->database().rollback();//回滚
          QMessageBox::warning(this,tr("tableModel"),tr("数据库错误:%1").arg(tabModel->lastError().text()));
          return;
        }
        setInfoLock(WINDOW_TABLE::MANAGE_STAFF,true);
        ui->btn_editStaffInfo->setText(tr("修改"));
    }
}

void MainWindow::on_btn_editVipInfo_clicked()
{
    if(infoLock)
    {
        //设置为可以修改
        setInfoLock(WINDOW_TABLE::MANAGE_VIP,false);
        ui->btn_editVipInfo->setText(tr("保存"));
    }
    else
    {
        setInfoLock(WINDOW_TABLE::MANAGE_VIP,true);
        ui->btn_editVipInfo->setText(tr("修改"));
    }
}


void MainWindow::on_btn_deleteVip_clicked()
{
    QModelIndex curIndex = theSelection->currentIndex();//获取当前选择单元格的模型索引
    QSqlRecord record = tabModel->record(curIndex.row());
    record.setValue("available",0);
    tabModel->setRecord(curIndex.row(),record);
    if(!tabModel->submitAll())
    {
      tabModel->database().rollback();//回滚
      QMessageBox::warning(this,tr("tableModel"),tr("数据库错误:%1").arg(tabModel->lastError().text()));
      return;
    }
}

void MainWindow::on_btn_deleteStaff_clicked()
{
    QModelIndex curIndex = theSelection->currentIndex();//获取当前选择单元格的模型索引
    QSqlRecord record = tabModel->record(curIndex.row());
    record.setValue("available",0);
    tabModel->setRecord(curIndex.row(),record);
    if(!tabModel->submitAll())
    {
      tabModel->database().rollback();//回滚
      QMessageBox::warning(this,tr("tableModel"),tr("数据库错误:%1").arg(tabModel->lastError().text()));
      return;
    }
}

void MainWindow::on_cb_payPro_activated(const QString &arg1)
{
    //打开数库
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    //新增一行
    int row_count = ui->tw_pay->rowCount();
    ui->tw_pay->insertRow(row_count);

    //获取显示项目名称
    QTableWidgetItem* item = new QTableWidgetItem(); //项目名
    QTableWidgetItem* item1 = new QTableWidgetItem();//原价
    QTableWidgetItem* item2 = new QTableWidgetItem();//折扣
    QTableWidgetItem* item3 = new QTableWidgetItem();//实价

    item->setText(arg1);
    ui->tw_pay->setItem(row_count,0,item);

    //获取显示折扣，原价
    QSqlQuery query1;
    QString select1 = QString("select pro_price,pro_discount from project where pro_name='%1'").arg(arg1);
    query1.exec(select1);
    double a,b;
    while(query1.next())
    {
        item1->setText(query1.value(0).toString());//原价
        a=query1.value(0).toDouble();
        ui->tw_pay->setItem(row_count,1,item1);
        item2->setText(query1.value(1).toString()+"%");//折扣，百分比
        b=query1.value(1).toDouble();
        ui->tw_pay->setItem(row_count,2,item2);
    }

    //实价
    double realprice=a*b/100;
    QString str = QString::number(realprice,'f',2);//类型转换，保留2为小数
    item3->setText(str);
    ui->tw_pay->setItem(row_count,4,item3);

    //理发师名字
    QComboBox* name = new QComboBox();
//    QString select2 = QString("select staff_name from staff where staff_project_id in(select pro_id from project where pro_name='%1')").arg(arg1);
    QString select2 = QString("select staff_name from staff where staff_project='%1'").arg(arg1);
    query1.exec(select2);
    while(query1.next())
    {
        name->addItem(query1.value(0).toString());
    }
    name->setCurrentIndex(-1);
    ui->tw_pay->setCellWidget(row_count,3,name);

    //计算显示总价
    double allcost=0;
    for(int i=0; i<=row_count; i++)
    {
        double temp=0;
        QString stemp;
        stemp=ui->tw_pay->item(i,4)->text();
        temp=stemp.toDouble();
        allcost+=temp;
    }
    QString Sallcost=QString::number(allcost,'f',2);
    ui->le_payAllCost->setText(Sallcost);

    //删除键
    QPushButton* btn_delete = new QPushButton();
    btn_delete->setText("删除");
    ui->tw_pay->setCellWidget(row_count,5,btn_delete);
    connect(btn_delete,&QPushButton::clicked,this,&MainWindow::btn_PayTableWidget_delete);

}

void MainWindow::btn_PayTableWidget_delete()
{
    int cur_row = ui->tw_pay->currentRow();
    ui->tw_pay->removeRow(cur_row);
    //更新总价
    double allcost=0;
    for(int i=0; i<ui->tw_pay->rowCount(); i++)
    {
        double temp=0;
        QString stemp;
        stemp=ui->tw_pay->item(i,4)->text();
        temp=stemp.toDouble();
        allcost+=temp;
    }
    QString Sallcost=QString::number(allcost,'f',2);
    ui->le_payAllCost->setText(Sallcost);
}

void MainWindow::on_btn_payPay_clicked()
{
    //打开数库
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    QSqlQuery query;

    QString no = ui->le_payCardNo->text();
    QString Allcost = ui->le_payAllCost->text();
    QString cost;
    QString project;
    QString staffno;
    QString staffname;
    //获取日期
    QDateTime current_time = QDateTime::currentDateTime();
    QString date = current_time.toString("yyyy/MM/dd");

    QString select1,select2;

    if(no=="")
    {
        QMessageBox message(QMessageBox::NoIcon,"错误","请输入会员卡号！");
        message.exec();
        return;
    }
    if(Allcost=="")
    {
        QMessageBox message(QMessageBox::NoIcon,"错误","消费项目不能为空");
        message.exec();
        return;
    }

    int allrows = ui->tw_pay->rowCount();
    for(int i=0; i<allrows; i++)
    {
        cost = ui->tw_pay->item(i,4)->text();
        project = ui->tw_pay->item(i,0)->text();

        //获取理发师名字
        QWidget* widget = ui->tw_pay->cellWidget(i,3);
        QComboBox* cbox = (QComboBox*)widget;
        staffname = cbox->currentText();

        //查找理发师编号
        select1="select staff_no from staff where staff_name='"+staffname+"'";
        query.exec(select1);
        if(query.next()) staffno=query.value(0).toString();


        //插入数据
        select2="insert into history(history_item_name,history_vip_no,history_staff_no,history_cost,history_time)""values('%1','%2','%3','%4','%5')";
        select2=select2.arg(project,no,staffno,cost,date);
        if(!query.exec(select2))
        {
            QMessageBox::warning(this,"添加错误","数据库错误！code:003");
            qDebug()<<"数据库错误！code:003";
            return;
        }

    }
    QMessageBox message(QMessageBox::NoIcon,"付款","付款成功");
    message.exec();

    for(int i=0; i<allrows; i++)
        ui->tw_pay->removeRow(ui->tw_pay->rowCount()-1);
    ui->cb_payPro->setCurrentIndex(-1);
    ui->le_payAllCost->setText("");
    ui->le_payCardNo->setText("");
}


bool MainWindow::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
    qDebug()<<sourceDir<<toDir;
    toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}


void MainWindow::on_save_triggered()
{
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QString filePath = QFileDialog::getExistingDirectory(this,"");//获取文件夹路径
    if(filePath.isEmpty())
    {
        QMessageBox::information(this,"信息","保存失败");
    }
    else
    {
        if(copyFileToPath(dbTools->getDbPath(),filePath+"/system.db",true)) QMessageBox::information(this,"提示","保存成功");
        else QMessageBox::warning(this,"失败","保存失败");
    }

}

void MainWindow::on_load_triggered()
{
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QString path = QFileDialog::getOpenFileName(NULL,"选择数据库文件",".","SQL Lite数据库(*.db *.db3)");
    if(path.isEmpty())
    {
        QMessageBox::information(this,"信息","保存失败");
        return;
    }
    dbTools->connect(path);
    //刷新页面
    ui->stackedWidget->setCurrentIndex(0);
    openTable(WINDOW_TABLE::MANAGE_STAFF);
    openHistoryTable(WINDOW_TABLE::MANAGE_STAFF);
}


void MainWindow::openPowerTable()//权限页面函数实现
{
    //不允许修改并只允许选中单行
    ui->tableWidget_powerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_powerTable->setSelectionBehavior(QAbstractItemView::SelectRows);//行选中
    ui->tableWidget_powerTable->setSelectionMode ( QAbstractItemView::SingleSelection);//单个选中
    //连接数据库，获取db对象
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    QSqlQuery query1;
    QString select1 = QString("select username,privilege from user");
    qDebug()<<query1.exec(select1);
    //删除原有行
    int lastRowCount = ui->tableWidget_powerTable->rowCount();
    for(int i=lastRowCount-1;i>=0;i--)
    {
        ui->tableWidget_powerTable->removeRow(i);
    }
    //刷新行
    int row_count = 0;
    while(query1.next())
    {
        QTableWidgetItem* item = new QTableWidgetItem(); //用户名
        QTableWidgetItem* item1 = new QTableWidgetItem();//权限
        ui->tableWidget_powerTable->insertRow(row_count);
        item->setText(query1.value(0).toString());
        ui->tableWidget_powerTable->setItem(row_count,0,item);
        item1->setText(query1.value(1).toString());
        ui->tableWidget_powerTable->setItem(row_count,1,item1);
        row_count++;
    }
}

void MainWindow::openSpendTable()
{
    //初始化表
    int allrow = ui->tw_spendTable->rowCount();
    for(int i=0; i<allrow; i++)
        ui->tw_spendTable->removeRow(ui->tw_spendTable->rowCount()-1);
    //打开数库
    DataBaseTools* dbTools = DataBaseTools::getDataBaseTools();
    QSqlDatabase db = dbTools->getDB();

    //获取cost月份
    QSqlQuery query;
    QString select="select cost_time from cost";
    QSet<QString> date;
    query.exec(select);
    while(query.next())
    {
        date.insert(query.value(0).toString().section("/",0,1));
    }

    //员工每月工资
    double salary=0;
    query.exec("select staff_salary from staff");
    while (query.next())
    {
        salary +=query.value(0).toDouble();
    }

    //查询对应月份数据
    auto point=date.begin();
    for(int i=0; i<date.size(); i++)
    {
//        qDebug()<<date.size()<<endl;
        //收入
        double in=0;
        query.exec("select history_cost from history where history_time like'"+(*point)+"%'");
        while (query.next())
        {
            in +=query.value(0).toDouble();
        }

        //支出
        double out=0;
        query.exec("select cost_money from cost where cost_time like'"+(*point)+"%'");
        while (query.next())
        {
            out +=query.value(0).toDouble();
        }
        out += salary;

        //利润
        double profit=in-out;

        //显示数据
        QTableWidgetItem *itemdate = new QTableWidgetItem();
        QTableWidgetItem *itemin = new QTableWidgetItem();
        QTableWidgetItem *itemout = new QTableWidgetItem();
        QTableWidgetItem *itemprofit = new QTableWidgetItem();
//        ,*itemin,*itemout,*itemprofit;
//        QString strp = QString::number(,'f',2)
        QString strin = QString::number(in,'f',2);
        QString strout = QString::number(out,'f',2);
        QString strprofit = QString::number(profit,'f',2);
        itemdate->setText(*point);
        itemin->setText(strin);
        itemout->setText(strout);
        itemprofit->setText(strprofit);
        ui->tw_spendTable->insertRow(i);
        ui->tw_spendTable->setItem(i,0,itemdate);
        ui->tw_spendTable->setItem(i,1,itemin);
        ui->tw_spendTable->setItem(i,2,itemout);
        ui->tw_spendTable->setItem(i,3,itemprofit);
        point++;
    }
}

void MainWindow::addrowPowerTable(QString username)//【权限页面函数实现】
{
    int row_count = ui->tableWidget_powerTable->rowCount();
    ui->tableWidget_powerTable->insertRow(row_count);
    qDebug()<<row_count;

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setText(username);
    ui->tableWidget_powerTable->setItem(row_count,0,item);//账号
    ui->tableWidget_powerTable->setItem(row_count,1,NULL);//权限
}

void MainWindow::on_btn_deleteUser_clicked()//删除键//【权限页面函数实现】
{
    int cur_row = ui->tableWidget_powerTable->currentRow();
    qDebug()<<cur_row;
    QString usernameStr = ui->tableWidget_powerTable->item(cur_row,0)->text();
    qDebug()<<usernameStr;
    QString delete_sql = "delete from user where username = ?";
    QSqlQuery sql_query;
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(usernameStr);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        qDebug()<<"deleted!";
    }
    ui->tableWidget_powerTable->removeRow(cur_row);
}
