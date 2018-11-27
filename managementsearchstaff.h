#ifndef MANAGEMENTSEARCHSTAFF_H
#define MANAGEMENTSEARCHSTAFF_H

#include <QMainWindow>

namespace Ui {
class ManagementSearchStaff;
}

class ManagementSearchStaff : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementSearchStaff(QWidget *parent = nullptr);
    ~ManagementSearchStaff();

private slots:
    void on_btn_search_clicked();

private:
    Ui::ManagementSearchStaff *ui;
};

#endif // MANAGEMENTSEARCHSTAFF_H
