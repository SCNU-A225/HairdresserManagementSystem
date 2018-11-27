#ifndef MANAGEMENTSEARCHVIP_H
#define MANAGEMENTSEARCHVIP_H

#include <QMainWindow>

namespace Ui {
class ManagementSearchVip;
}

class ManagementSearchVip : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementSearchVip(QWidget *parent = nullptr);
    ~ManagementSearchVip();

private slots:
    void on_btn_search_clicked();

private:
    Ui::ManagementSearchVip *ui;
};

#endif // MANAGEMENTSEARCHVIP_H
