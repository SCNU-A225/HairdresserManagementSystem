#ifndef MANAGEMENTADDSTAFF_H
#define MANAGEMENTADDSTAFF_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class ManagementAddStaff;
}

class ManagementAddStaff : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementAddStaff(QWidget *parent = nullptr);
    ~ManagementAddStaff();

private slots:
    void on_btn_submit_clicked();

private:
    Ui::ManagementAddStaff *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // MANAGEMENTADDSTAFF_H
