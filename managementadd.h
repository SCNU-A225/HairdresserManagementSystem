#ifndef MANAGEMENTADD_H
#define MANAGEMENTADD_H

#include <QMainWindow>

namespace Ui {
class ManagementAdd;
}

class ManagementAdd : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementAdd(QWidget *parent = nullptr);
    ~ManagementAdd();

private:
    Ui::ManagementAdd *ui;
};

#endif // MANAGEMENTADD_H
