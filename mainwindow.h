#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QItemSelectionModel>

enum WINDOW_TABLE{MANAGE_STAFF,MANAGE_VIP,STATISTIC_STAFF,STATISTIC_VIP};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSqlTableModel* getTaModel(){return tabModel;}
    void openTable(WINDOW_TABLE wt);//从数据库读取信息加到表格中
    void openSpendTable();//开支表显示
    explicit MainWindow(QWidget *parent = nullptr);
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);//复制文件
    void addrowPowerTable(QString username);//权限函数定义
    ~MainWindow();

private slots:
    void on_management_triggered();

    void on_pay_triggered();

    void on_spend_triggered();

    void on_statistics_triggered();

    void on_power_triggered();

    void on_btn_changeFare_clicked();

    void on_btn_addSpend_clicked();

    void on_btn_printForm_clicked();

    void on_btn_addUser_clicked();

    void on_btn_searchStaff_clicked();

    void on_btn_addStaff_clicked();

    void on_btn_searchVip_clicked();

    void on_btn_addVip_clicked();

    void on_btn_addVip2_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);// QTableView的SelectionModel的行发生了变化，进行处理

    void on_btn_editStaffInfo_clicked();

    void on_btn_editVipInfo_clicked();

    void on_btn_deleteVip_clicked();

    void on_btn_deleteStaff_clicked();

    void on_tabWidget_2_currentChanged(int index);

    void on_cb_payPro_activated(const QString &arg1);

    void btn_PayTableWidget_delete();

    void on_btn_payPay_clicked();

    void on_save_triggered();

    void on_load_triggered();

    void on_btn_deleteUser_clicked();

private:
    Ui::MainWindow *ui;
    QDataWidgetMapper *dataMapper; //数据映射
    QItemSelectionModel* theSelection;//选择模型
    QSqlTableModel *tabModel;//表格模型
    bool infoLock;
    void switchPage(int index);//切换界面中stackWidget的页面
    void openHistoryTable(WINDOW_TABLE wt);
    void setInfoLock(WINDOW_TABLE wt,bool lock);//设置是否锁定管理页信息
    void openPowerTable();//权限函数定义


};

#endif // MAINWINDOW_H
