#ifndef MAKEACT_H
#define MAKEACT_H

#include <QDate>
#include <QDialog>
#include <thread>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QLineEdit>
#include <QSettings>
#include <QLayout>
#include <QPushButton>
#include <QCoreApplication>

namespace Ui {
class MakeAct;
}

class MakeAct : public QDialog
{
    Q_OBJECT

public:
    explicit MakeAct(QWidget *parent = nullptr);
    ~MakeAct();

protected slots:
    void add_route_to_table();
    void remove_route_from_table();
    void write_routes();

private slots:
    void inline update_list();
    void make_report();

private:
    QDialog *addRouteDialog = new QDialog;
    QGridLayout *addRouteLayout = new QGridLayout;
    QLineEdit *route = new QLineEdit;
    QPushButton *accept = new QPushButton("Принять");
    QPushButton *reject = new QPushButton("Отмена");

    Ui::MakeAct *ui;
};

#endif // MAKEACT_H
