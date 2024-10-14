#ifndef MAKEACT_H
#define MAKEACT_H

#include <QDate>
#include <QDialog>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <QDebug>
#include <QLineEdit>
#include <QSettings>
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCoreApplication>
#include <boost/json.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include "xlnt/xlnt.hpp"

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
    Q_SLOT void start_make_report();

    std::vector<std::vector<std::string>> make_route_report(const std::string&);
    void download_act();

private:
    Q_SLOT void exit();

    QDialog *addRouteDialog = new QDialog;
    QGridLayout *addRouteLayout = new QGridLayout;
    QLineEdit *route = new QLineEdit;
    QPushButton *accept = new QPushButton("Принять");
    QPushButton *reject = new QPushButton("Отмена");

    Ui::MakeAct *ui;
};

#endif // MAKEACT_H
