#ifndef MANAGEMENTADDVIP_H
#define MANAGEMENTADDVIP_H

#include <QMainWindow>
#include <QCloseEvent>

namespace Ui {
class ManagementAddVip;
}

class ManagementAddVip : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementAddVip(QWidget *parent = nullptr);
    ~ManagementAddVip();

private slots:
    void on_submit_clicked();

private:
    Ui::ManagementAddVip *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // MANAGEMENTADDVIP_H
