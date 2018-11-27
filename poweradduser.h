#ifndef POWERADDUSER_H
#define POWERADDUSER_H

#include <QMainWindow>

namespace Ui {
class PowerAddUser;
}

class PowerAddUser : public QMainWindow
{
    Q_OBJECT

public:
    explicit PowerAddUser(QWidget *parent = nullptr);
    ~PowerAddUser();

private slots:
    void on_pushButton_enter_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::PowerAddUser *ui;
};

#endif // POWERADDUSER_H
