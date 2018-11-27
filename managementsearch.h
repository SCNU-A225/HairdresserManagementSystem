#ifndef MANAGEMENTSEARCH_H
#define MANAGEMENTSEARCH_H

#include <QMainWindow>

namespace Ui {
class ManagementSearch;
}

class ManagementSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagementSearch(QWidget *parent = nullptr);
    ~ManagementSearch();

private:
    Ui::ManagementSearch *ui;
};

#endif // MANAGEMENTSEARCH_H
