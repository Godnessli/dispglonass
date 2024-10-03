#ifndef MAKEACT_H
#define MAKEACT_H

#include <QDate>
#include <QDialog>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QLineEdit>
#include <QSettings>
#include <QLayout>
#include <QPushButton>
#include <QCoreApplication>
#include <boost/python.hpp>

namespace Ui {
class MakeAct;
}

class MakeAct : public QDialog
{
    Q_OBJECT

public:
    explicit MakeAct(QWidget *parent = nullptr);
    ~MakeAct();

protected:
    Q_SLOT void add_route_to_table();
    Q_SLOT void remove_route_from_table();
    Q_SLOT void write_routes();
private:
    Q_SLOT void inline update_list();
    Q_SLOT void make_report();
    void download_act();

private:
    QDialog *addRouteDialog = new QDialog;
    QGridLayout *addRouteLayout = new QGridLayout;
    QLineEdit *route = new QLineEdit;
    QPushButton *accept = new QPushButton("Принять");
    QPushButton *reject = new QPushButton("Отмена");

    Ui::MakeAct *ui;
};

#endif // MAKEACT_H
